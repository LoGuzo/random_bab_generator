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

// ���ڿ� ó�� ��ƿ��Ƽ �Լ�
void init_string(struct string* ); // string �ʱ�ȭ �Լ�
char* encoder(const char*, int); // ���ڴ� �Լ�
size_t writefunc(void*, size_t, size_t, struct string*); // writefunc ���� �Լ�
int parse_members_from_text(const char*, LPARRAY); // text���� �̸� ���� �Լ�
char* merge_members_text(LPARRAY, int); // text�պ� �Լ�
char* safe_strtok(char* str, const char* delim, char** context); // ��Ƽ �÷��� strtok�Լ�

// api ó���Լ�
void request_API(SlackChannel*, LPARRAY, LPARRAY); // api ��û �Լ�
char* get_token_from_file(const char*); // bot_token Ȯ�� �Լ�
int slack_fetch_channels(const char*, SlackChannel*, int*); // ä�� �������� �Լ�
void slack_user_name_by_id(const char*, const char*, LPARRAY); // id���� �̸� ���� �Լ�
void slack_conversation_members(const char*, const char*, LPARRAY); // �ο� ���� �Լ�
void slack_send_message(char*, const char*, const char*); // �޼��� ������ �Լ�
void slack_recent_message(char*, const char*, LPARRAY); // �ֱ� �޼��� �������� �Լ�


#endif // !SLACK_API_H
