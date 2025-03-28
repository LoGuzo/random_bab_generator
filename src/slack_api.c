#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../inc/slack_api.h"
#include <curl/curl.h>
#include <jansson.h>

#define MAX_CHANNELS 100
#define MAX_MEMBERS 200

void init_string(struct string* s) {
    s->len = 0;
    s->ptr = malloc(1);
    if (s->ptr == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    s->ptr[0] = '\0';
}

// 인코더 0입력시 ACP->UTF8 1입력시 CP_ACP
char* encoder(const char* input, int encode) {
    int from = (encode == 1) ? CP_ACP : CP_UTF8;
    int to = (encode == 1) ? CP_UTF8 : CP_ACP;

    int wlen = MultiByteToWideChar(from, 0, input, -1, NULL, 0);
    if (wlen <= 0) return NULL;

    wchar_t* wstr = (wchar_t*)malloc(wlen * sizeof(wchar_t));
    if (!wstr) return NULL;

    MultiByteToWideChar(from, 0, input, -1, wstr, wlen);

    int u8len = WideCharToMultiByte(to, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (u8len <= 0) {
        free(wstr);
        return NULL;
    }

    char* utf8 = (char*)malloc(u8len);
    if (!utf8) {
        free(wstr);
        return NULL;
    }

    WideCharToMultiByte(to, 0, wstr, -1, utf8, u8len, NULL, NULL);
    free(wstr);
    return utf8;
}

size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "메모리 재할당 실패\n");
        exit(1);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

char* get_token_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    static char token[256];
    if (!file) {
        perror("config.txt 파일을 열 수 없습니다");
        return NULL;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "SLACK_BOT_TOKEN=", 16) == 0) {
            strncpy(token, line + 16, sizeof(token));
            token[strcspn(token, "\n")] = '\0'; // 개행 제거
            fclose(file);
            return token;
        }
    }

    fclose(file);
    return NULL;
}

int slack_fetch_channels(const char* token, SlackChannel* channels, int* channel_count) {
    CURL* curl = curl_easy_init();
    struct curl_slist* headers = NULL;
    struct string response;
    init_string(&response);

    if (!curl) {
        fprintf(stderr, "curl 초기화 실패\n");
        return 0;
    }

    char auth_header[300];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, "https://slack.com/api/conversations.list");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "API 요청 실패: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return;
    }

    // JSON 파싱
    json_error_t error;
    json_t* root = json_loads(response.ptr, 0, &error);
    if (!root) {
        fprintf(stderr, "JSON 파싱 에러: %s\n", error.text);
        return;
    }

    json_t* ok = json_object_get(root, "ok");
    if (!json_is_true(ok)) {
        fprintf(stderr, "Slack API 호출 실패: %s\n", response.ptr);
        return;
    }

    json_t* chs = json_object_get(root, "channels");
    if (!json_is_array(chs)) {
        fprintf(stderr, "channels 배열이 없음\n");
        return;
    }

    size_t index;
    json_t* chan;
    int i = 0;
    json_array_foreach(chs, index, chan) {
        const char* id = json_string_value(json_object_get(chan, "id"));
        const char* name = json_string_value(json_object_get(chan, "name"));

        if (id && name && i < MAX_CHANNELS) {
            strncpy(channels[i].id, encoder(id, 0), sizeof(channels[i].id));;
            strncpy(channels[i].name, encoder(name, 0), sizeof(channels[i].name));;
            i++;
        }
    }

    *channel_count = i;

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(response.ptr);
    json_decref(root);

    return 1;
}
void slack_user_name_by_id(const char* user_id, const char* token) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    char url[512];
    snprintf(url, sizeof(url), "https://slack.com/api/users.info?user=%s", user_id);

    struct curl_slist* headers = NULL;
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

        json_t* root = json_loads(response.ptr, 0, &error);
        if (!root) {
            fprintf(stderr, "JSON 파싱 에러: %s\n", error.text);
            return 0;
        }

        json_t* ok = json_object_get(root, "ok");
        if (!json_is_true(ok)) {
            fprintf(stderr, "Slack API 호출 실패: %s\n", response.ptr);
            return 0;
        }

        json_t* user = json_object_get(root, "user");
        if (!json_is_object(user)) {
            fprintf(stderr, "User 없음.\n");
            return 0;
        }

        json_t* is_bot = json_object_get(user, "is_bot");

        if (json_is_true(is_bot))
            return 0;

        json_t* profile = json_object_get(user, "profile");

        if (!json_is_object(profile)) {
            fprintf(stderr, "Profile 없음.\n");
            return 0;
        }

        const char* name = json_string_value(json_object_get(profile, "display_name"));
        if (!name)
            name = json_string_value(json_object_get(profile, "real_name"));
        SetConsoleOutputCP(CP_UTF8);
        printf("사용자 이름: %s\n", name);

        json_decref(root);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    free(response.ptr);
}

void slack_conversation_members(const char* channel_id, const char* token) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    char url[512];
    snprintf(url, sizeof(url), "https://slack.com/api/conversations.members?channel=%s", channel_id);

    struct curl_slist* headers = NULL;
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

        json_t* root = json_loads(response.ptr, 0, &error);
        if (!root) {
            fprintf(stderr, "JSON 파싱 에러: %s\n", error.text);
            return;
        }

        json_t* ok = json_object_get(root, "ok");
        if (!json_is_true(ok)) {
            fprintf(stderr, "Slack API 호출 실패: %s\n", response.ptr);
            return;
        }

        json_t* members = json_object_get(root, "members");
        if (!json_is_array(members)) {
            fprintf(stderr, "members 배열이 없음 : %s\n", response.ptr);
            return;
        }

        size_t index;
        json_t* member;
        json_array_foreach(members, index, member) {
            const char* user_id = json_string_value(member);
            if (user_id) {
                slack_user_name_by_id(user_id, token); // 밑에 정의될 함수
            }
        }
        json_decref(root);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    free(response.ptr);
}

void slack_send_message(char* channel_id, const char* token, const char* text) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    const char* url = "https://slack.com/api/chat.postMessage";

    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
        "{\"channel\":\"%s\", \"text\":\"<!channel>%s\"}", channel_id, encoder(text, 1));

    struct curl_slist* headers = NULL;
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);

    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
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
        fprintf(stderr, "Slack 전송 실패: %s\n", curl_easy_strerror(response));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void slack_recent_message(char* channel_id, const char* token) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    char url[512];
    snprintf(url, sizeof(url), "https://slack.com/api/conversations.history?channel=%s&limit=1", channel_id);

    struct curl_slist* headers = NULL;
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

        json_t* root = json_loads(response.ptr, 0, &error);
        if (!root) {
            fprintf(stderr, "JSON 파싱 에러: %s\n", error.text);
            return 0;
        }

        json_t* ok = json_object_get(root, "ok");
        if (!json_is_true(ok)) {
            fprintf(stderr, "Slack API 호출 실패: %s\n", response.ptr);
            return 0;
        }

        json_t* messages = json_object_get(root, "messages");
        if (!json_is_array(messages)) {
            fprintf(stderr, "message 배열 비어있음\n");
            return 0;
        }
        const char* text = json_string_value(json_object_get(messages, "text"));

        size_t index;
        json_t* texts;
        json_array_foreach(messages, index, texts) {
            const char* text = json_string_value(json_object_get(texts, "text"));
            if (text) {
                printf("%d text: %s\n", index, text);
            }
        }

        json_decref(root);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    free(response.ptr);
}