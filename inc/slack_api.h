#include "array.h"
#ifndef SLACK_API_H
#define SLACK_API_H

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
void init_string(struct string* s);
char* encoder(const char* input, int encode);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);

// api 처리함수
void request_API(SlackChannel* channels, LPARRAY* slack_members);
char* get_token_from_file(const char* filename);    
int slack_fetch_channels(const char* token, SlackChannel* channels, int* channel_count);
void slack_user_name_by_id(const char* user_id, const char* token, LPARRAY* slack_members);
void slack_conversation_members(const char* channel_id, const char* token, LPARRAY* slack_members);
void slack_send_message(char* channel_id, const char* token, const char* text);
void slack_recent_message(char* channel_id, const char* token);

#endif // !SLACK_API_H
