#include "array.h"
#include "shuffle.h"
#include "slack_api.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int group_size = 4;
  LPARRAY members;
  LPARRAY except_members;
  LPARRAY lastweak_group_members;
  SlackChannel channels[ARRAR_INIT_SIZE];

  arrayCreate(&members);
  arrayCreate(&except_members);
  arrayCreate(&lastweak_group_members);

  char *channel_id = argv[1];
  if (argc > 1) {
    group_size = atoi(argv[2]);
    for (int i = 3; i < argc; i++) {
      arrayAdd(except_members, argv[i]);
    }
  }

  request_API(channel_id, channels, members, lastweak_group_members,
              except_members, group_size);

  arrayDestroy(members);
  arrayDestroy(except_members);
  arrayDestroy(lastweak_group_members);
  return 0;
}
