#include "array.h"
#ifndef SLACK_API_H
#define SLACK_API_H

#define MAX_CHANNELS 100
#define MAX_MEMBERS 200

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
void init_string(struct string* );
char* encoder(const char*, int);
size_t writefunc(void*, size_t, size_t, struct string*);
int parse_members_from_text(const char*, LPARRAY);
char* merge_members_text(LPARRAY, int);

// api 처리함수
void request_API(SlackChannel*, LPARRAY, LPARRAY);
char* get_token_from_file(const char*);    
int slack_fetch_channels(const char*, SlackChannel*, int*);
void slack_user_name_by_id(const char*, const char*, LPARRAY);
void slack_conversation_members(const char*, const char*, LPARRAY);
void slack_send_message(char*, const char*, const char*);
void slack_recent_message(char*, const char*, LPARRAY);

#endif // !SLACK_API_H
