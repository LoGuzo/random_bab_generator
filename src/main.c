#include "../inc/array.h"
#include "../inc/header.h"
#include "../inc/shuffle.h"
#include "../inc/slack_api.h"

int main(int argc, char *argv[]) {
	LPARRAY members[ARRAR_INIT_SIZE];
	LPARRAY group_members[ARRAR_INIT_SIZE];
	LPARRAY lastweak_group_members[ARRAR_INIT_SIZE];
	SlackChannel channels[ARRAR_INIT_SIZE];

	arrayCreate(&members);
	arrayCreate(&group_members);
	arrayCreate(&lastweak_group_members);
	
	request_API(&channels, &members);

	shuffle_member(members);

	arrayDestroy(members);
	return 0;
}
