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
    char id[64];
    char name[128];
} SlackMember;

void init_string(struct string* s);
char* encoder(const char* input, int encode);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);
char* get_token_from_file(const char* filename);    
int slack_fetch_channels(const char* token, SlackChannel* channels, int* channel_count);
void slack_user_name_by_id(const char* user_id, const char* token);
void slack_conversation_members(const char* channel_id, const char* token);
void slack_send_message(char* channel_id, const char* token, const char* text);
void slack_recent_message(char* channel_id, const char* token);

#endif // !SLACK_API_H
