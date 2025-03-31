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
    //    // 1��
    //    "�Ǻ���", "�嵿ö", "������", "������",
    //    // 2��
    //    "����", "����ȣ", "�����",
    //    // 3��
    //    "��濬", "������", "�տ��", "���ٺ�",
    //    // 4��
    //    "�̱�â", "����ȯ", "������",
    //    // 5��
    //    "������", "Ȳ��ȣ", "�ѿ���", "������",
    //    // 6��
    //    "������", "������", "������", "������",
    //    // 7��
    //    "�赵��", "������", "������", "������"
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
