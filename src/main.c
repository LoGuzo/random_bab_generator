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
	
    //const char* names[] = {
    //    // 1조
    //    "권병수", "장동철", "장현우", "남윤서",
    //    // 2조
    //    "목경민", "추은호", "김민정",
    //    // 3조
    //    "김경연", "조찬우", "손요셉", "조다빈",
    //    // 4조
    //    "이기창", "강시환", "윤영두",
    //    // 5조
    //    "조수빈", "황주호", "한영서", "오상준",
    //    // 6조
    //    "김유선", "이현지", "조정현", "서유진",
    //    // 7조
    //    "김도현", "문예성", "류예지", "이재형"
    //};

    //const int count = sizeof(names) / sizeof(names[0]);

    //for (int i = 0; i < count; i++) {
    //    SlackMember* member = malloc(sizeof(SlackMember));
    //    if (!member) {
    //        fprintf(stderr, "Memory Allocate Fail\n");
    //        exit(1);
    //    }
    //    strncpy(member->name, names[i], sizeof(member->name) - 1);
    //    member->name[sizeof(member->name) - 1] = '\0';
    //    arrayAdd(members, (LPDATA)member);
    //}

	request_API(&channels, members, lastweak_group_members);

	arrayDestroy(members);
	arrayDestroy(lastweak_group_members);
	return 0;
}
