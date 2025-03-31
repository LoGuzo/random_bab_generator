#ifndef SLACK_API_H
#define SLACK_API_H

#include "array.h"

#define MAX_CHANNELS 100
#define MAX_MEMBERS 200

#ifdef _WIN32
#else
#include <unitypes.h>
#endif

struct string {
    char* ptr;
    size_t len;
};

typedef struct {
    char id[64];
    char name[128];
} SlackChannel;

typedef struct {
    char name[128];
} SlackMember;

// 문자열 처리 유틸리티 함수
void init_string(struct string* ); // string 초기화 함수
char* encoder(const char*, int); // 인코더 함수
size_t writefunc(void*, size_t, size_t, struct string*); // writefunc 제작 함수
int parse_members_from_text(const char*, LPARRAY); // text에서 이름 추출 함수
char* merge_members_text(LPARRAY, int); // text합병 함수
char* safe_strtok(char* str, const char* delim, char** context); // 멀티 플랫폼 strtok함수

// api 처리함수
void request_API(char*, SlackChannel*, LPARRAY, LPARRAY); // api 요청 함수
char* get_token_from_file(const char*); // bot_token 확인 함수
void slack_user_name_by_id(const char*, const char*, LPARRAY); // id에서 이름 추출 함수
void slack_conversation_members(const char*, const char*, LPARRAY); // 인원 추출 함수
void slack_send_message(char*, const char*, const char*); // 메세지 보내기 함수
void slack_recent_message(char*, const char*, LPARRAY); // 최근 메세지 가져오기 함수


#endif // !SLACK_API_H
