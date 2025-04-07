#include "slack_api.h"
#include "shuffle.h"
#include <curl/curl.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <libgen.h>
#include <limits.h>
#include <locale.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <wchar.h>
#endif

// string 초기화
void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(1);
  if (s->ptr == NULL) {
    fprintf(stderr, "Memory Allocate Fail\n");
    exit(1);
  }
  s->ptr[0] = '\0';
}

char *safe_strtok(char *str, const char *delim, char **context) {
#ifdef _MSC_VER
  return strtok_s(str, delim, context);
#else
  return strtok_r(str, delim, context);
#endif
}

// 인코더 1 입력시 ACP->UTF8, 0 입력시 UTF8 -> ACP
char *encoder(const char *input, int encode) {
#ifdef _WIN32
  int from = (encode == 1) ? CP_ACP : CP_UTF8;
  int to = (encode == 1) ? CP_UTF8 : CP_ACP;

  int wlen = MultiByteToWideChar(from, 0, input, -1, NULL, 0);
  if (wlen <= 0)
    return NULL;

  wchar_t *wstr = (wchar_t *)malloc(sizeof(wchar_t) * wlen);
  if (!wstr)
    return NULL;

  MultiByteToWideChar(from, 0, input, -1, wstr, wlen);

  int u8len = WideCharToMultiByte(to, 0, wstr, -1, NULL, 0, NULL, NULL);
  if (u8len <= 0) {
    free(wstr);
    return NULL;
  }

  char *result = (char *)malloc(u8len);
  if (!result) {
    free(wstr);
    return NULL;
  }

  WideCharToMultiByte(to, 0, wstr, -1, result, u8len, NULL, NULL);
  free(wstr);
  return result;

#else // Linux, Unix

  // 로캘 세팅 (한 번만 호출해도 무방하긴 함)
  setlocale(LC_ALL, "");

  // Step 1: 멀티바이트 → 와이드
  size_t wlen = mbstowcs(NULL, input, 0);
  if (wlen == (size_t)-1)
    return NULL;

  wchar_t *wstr = (wchar_t *)malloc((wlen + 1) * sizeof(wchar_t));
  if (!wstr)
    return NULL;

  mbstowcs(wstr, input, wlen + 1);

  // Step 2: 와이드 → 멀티바이트 (다른 방향)
  size_t mblen = wcstombs(NULL, wstr, 0);
  if (mblen == (size_t)-1) {
    free(wstr);
    return NULL;
  }

  char *result = (char *)malloc(mblen + 1);
  if (!result) {
    free(wstr);
    return NULL;
  }

  wcstombs(result, wstr, mblen + 1);
  free(wstr);

  return result;

#endif
}

// json파일 파싱시 사용할 wirtefunc 제작
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t new_len = s->len + size * nmemb;
  s->ptr = realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "Memory Allocate Fail\n");
    exit(1);
  }
  memcpy(s->ptr + s->len, ptr, size * nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;
  return size * nmemb;
}

// 텍스트에서 인원을 추출
int parse_members_from_text(const char *text, LPARRAY last_week_members) {
  char temp[2048];
  strncpy(temp, text, sizeof(temp) - 1);
  temp[sizeof(temp) - 1] = '\0';

  char *context_line = NULL;
  char *line = safe_strtok(temp, "\n", &context_line);
  int line_num = 0;

  while (line != NULL) {
    if (line_num == 0) {
      line = safe_strtok(NULL, "\n", &context_line);
      line_num++;
      continue;
    }

    char *colon = strchr(line, ':');
    if (!colon) {
      line = safe_strtok(NULL, "\n", &context_line);
      line_num++;
      continue;
    }

    char *names = colon + 1;
    char *context_name = NULL;
    char *name_token = safe_strtok(names, ",", &context_name);

    while (name_token) {
      // 앞뒤 공백 제거
      while (*name_token == ' ')
        name_token++;
      char *end = name_token + strlen(name_token) - 1;
      while (end > name_token && (*end == ' ' || *end == '\n' || *end == '\r'))
        *end-- = '\0';

      SlackMember *member = (SlackMember *)malloc(sizeof(SlackMember));
      strcpy(member->name, name_token);
      arrayAdd(last_week_members, member);

      name_token = safe_strtok(NULL, ",", &context_name);
    }

    line = safe_strtok(NULL, "\n", &context_line);
    line_num++;
  }

  return 0;
}

// 배열에있는 이름을 1조 : 홍길동, ... 과 같이 바꾸어 합병
char *merge_members_text(LPARRAY slack_members, int group_size) {
  int text_size = 2048;
  char *merge_text = (char *)malloc(text_size);
  if (!merge_text)
    return NULL;

  merge_text[0] = '\0'; // 초기화

  int group_cnt = (slack_members->size + group_size - 1) / group_size;
  int offset = 0;
  int group_num = 1;

  for (int i = 0; i < slack_members->size; i++) {
    if (i % group_size == 0) {
      offset += snprintf(merge_text + offset, text_size - offset,
                         "#%d : ", group_num++);
    }

    SlackMember *member = (SlackMember *)(slack_members->lpData[i]);
    offset +=
        snprintf(merge_text + offset, text_size - offset, "%s", member->name);

    if ((i + 1) % group_size != 0 && i != slack_members->size - 1) {
      offset += snprintf(merge_text + offset, text_size - offset, ", ");
    } else {
      offset += snprintf(merge_text + offset, text_size - offset, "\n");
    }
  }

  return merge_text;
}

// slack에 각 기능을 사용하기 위한 api 요청
void request_API(char *channel_id, SlackChannel *channels,
                 LPARRAY slack_members, LPARRAY last_week_members,
                 LPARRAY except_members, int group_size, const char *token) {
  int channel_count = 0;

  slack_conversation_members(channel_id, token, slack_members, except_members);

  slack_recent_message(channel_id, token, last_week_members);

  assign_memeber(slack_members, last_week_members, group_size);

  char *merge_text = merge_members_text(slack_members, group_size);
  printf("%s", merge_text);

  free(merge_text);
  return;
}

// 유저 id를 사용하여 유저의 이름 가져오기
void slack_user_name_by_id(const char *user_id, const char *token,
                           LPARRAY slack_members, LPARRAY except_members) {
  CURL *curl = curl_easy_init();
  if (!curl)
    return;

  char url[512];
  snprintf(url, sizeof(url), "https://slack.com/api/users.info?user=%s",
           user_id);

  struct curl_slist *headers = NULL;
  struct string response;
  init_string(&response);
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  CURLcode res = curl_easy_perform(curl);
  if (res == CURLE_OK) {
    json_error_t error;

    json_t *root = json_loads(response.ptr, 0, &error);
    if (!root) {
      fprintf(stderr, "JSON Parse Error: %s\n", error.text);
      return;
    }

    json_t *ok = json_object_get(root, "ok");
    if (!json_is_true(ok)) {
      fprintf(stderr, "Slack API Call Fail: %s\n", response.ptr);
      return;
    }

    json_t *user = json_object_get(root, "user");
    if (!json_is_object(user)) {
      fprintf(stderr, "NO User.\n");
      return;
    }

    json_t *is_bot = json_object_get(user, "is_bot");

    if (json_is_true(is_bot))
      return;

    json_t *is_admin = json_object_get(user, "is_admin");

    if (json_is_true(is_admin))
      return;

    json_t *profile = json_object_get(user, "profile");

    if (!json_is_object(profile)) {
      fprintf(stderr, "NO Profile.\n");
      return;
    }

    const char *name =
        json_string_value(json_object_get(profile, "display_name"));
    if (!name)
      name = json_string_value(json_object_get(profile, "real_name"));

    char *encod_name = encoder(name, 0);

    for (int i = 0; i < except_members->size; i++) {
      char *except_member = (char *)except_members->lpData[i];
      if (!strcmp(encod_name, except_member))
        return;
    }

    SlackMember *member = (SlackMember *)malloc(sizeof(SlackMember));
    strcpy(member->name, encod_name);

    arrayAdd(slack_members, member);

    json_decref(root);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  free(response.ptr);
}

// 대화 채널 내 멤버들 아이디 가져오기
void slack_conversation_members(const char *channel_id, const char *token,
                                LPARRAY slack_members, LPARRAY except_members) {
  CURL *curl = curl_easy_init();
  if (!curl)
    return;

  char url[512];
  snprintf(url, sizeof(url),
           "https://slack.com/api/conversations.members?channel=%s",
           channel_id);

  struct curl_slist *headers = NULL;
  struct string response;
  init_string(&response);
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  CURLcode res = curl_easy_perform(curl);
  if (res == CURLE_OK) {
    json_error_t error;

    json_t *root = json_loads(response.ptr, 0, &error);
    if (!root) {
      fprintf(stderr, "JSON Parse Error: %s\n", error.text);
      return;
    }

    json_t *ok = json_object_get(root, "ok");
    if (!json_is_true(ok)) {
      fprintf(stderr, "Slack API Call Fail: %s\n", response.ptr);
      return;
    }

    json_t *members = json_object_get(root, "members");
    if (!json_is_array(members)) {
      fprintf(stderr, "NO Members Array : %s\n", response.ptr);
      return;
    }

    size_t index;
    json_t *member;
    json_array_foreach(members, index, member) {
      const char *user_id = json_string_value(member);
      if (user_id) {
        slack_user_name_by_id(user_id, token, slack_members,
                              except_members); // 밑에 정의될 함수
      }
    }
    json_decref(root);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  free(response.ptr);
}

// 합병했던 text를 slack에 전송
void slack_send_message(char *channel_id, const char *token, const char *text) {
  CURL *curl = curl_easy_init();
  if (!curl)
    return;

  const char *url = "https://slack.com/api/chat.postMessage";

  char post_data[1024];
  snprintf(post_data, sizeof(post_data),
           "{\"channel\":\"%s\", \"text\":\"%s\"}", channel_id, text);

  char *encoded = encoder(post_data, 1);
  if (encoded) {
    strncpy(post_data, encoded, sizeof(post_data) - 1);
    post_data[sizeof(post_data) - 1] = '\0';
    free(encoded);
  }

  struct curl_slist *headers = NULL;
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);

  headers = curl_slist_append(headers,
                              "Content-Type: application/json; charset=utf-8");
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  CURLcode response = curl_easy_perform(curl);
  if (response != CURLE_OK) {
    fprintf(stderr, "Slack Send Fail: %s\n", curl_easy_strerror(response));
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
}

// 최근 메세지 가져오기
void slack_recent_message(char *channel_id, const char *token,
                          LPARRAY last_week_members) {
  CURL *curl = curl_easy_init();
  if (!curl)
    return;

  char url[512];
  snprintf(url, sizeof(url),
           "https://slack.com/api/conversations.history?channel=%s&limit=5",
           channel_id);

  struct curl_slist *headers = NULL;
  struct string response;
  init_string(&response);
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  CURLcode res = curl_easy_perform(curl);
  if (res == CURLE_OK) {
    json_error_t error;

    json_t *root = json_loads(response.ptr, 0, &error);
    if (!root) {
      fprintf(stderr, "JSON Parse Error: %s\n", error.text);
      return;
    }

    json_t *ok = json_object_get(root, "ok");
    if (!json_is_true(ok)) {
      fprintf(stderr, "Slack API Call Fail: %s\n", response.ptr);
      return;
    }

    json_t *messages = json_object_get(root, "messages");
    if (!json_is_array(messages)) {
      fprintf(stderr, "NO message array\n");
      return;
    }

    size_t index;
    json_t *texts;
    json_array_foreach(messages, index, texts) {
      const char *text = json_string_value(json_object_get(texts, "text"));

      text = encoder(text, 0);
      if (text && strstr(text, "#1")) {
        parse_members_from_text(text, last_week_members);
        break; // 첫 번째 !channel 메시지만 처리하고 종료
      }
    }

    json_decref(root);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  free(response.ptr);
}
