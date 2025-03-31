#include "../inc/array.h"
#include "../inc/header.h"
#include "../inc/shuffle.h"
#include "../inc/slack_api.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int group_size = 0;
	LPARRAY members;
	LPARRAY except_members;
	LPARRAY lastweak_group_members;
	SlackChannel channels[ARRAR_INIT_SIZE];

	arrayCreate(&members);
	arrayCreate(&except_members);
	arrayCreate(&lastweak_group_members);

	arrayAdd(except_members, "chk");

	request_API("C08K4LEULF4", channels, members, lastweak_group_members, except_members, group_size);

	arrayDestroy(members);
	arrayDestroy(except_members);
	arrayDestroy(lastweak_group_members);
	return 0;
}
