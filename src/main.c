#include "../inc/array.h"
#include "../inc/header.h"
#include "../inc/shuffle.h"
#include "../inc/slack_api.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	LPARRAY members;
	LPARRAY lastweak_group_members;
	SlackChannel channels[ARRAR_INIT_SIZE];

	arrayCreate(&members);
	arrayCreate(&lastweak_group_members);

    request_API(argv[1], channels, members, lastweak_group_members);

	arrayDestroy(members);
	arrayDestroy(lastweak_group_members);
	return 0;
}
