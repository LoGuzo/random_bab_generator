#include "shuffle.h"
#include <time.h>
#include <stdlib.h>

void shuffle_member(char** members, int member_cnt)
{
	for (int i = member_cnt - 1;i > 0;i--) {
		int j = rand() % (i + 1);
		swap(&members[i], &members[j]);
	}
}

void swap(char** name1, char** name2)
{
	char* temp = *name1;
	*name1 = *name2;
	*name2 = temp;
}

void assign_member(char** members, int member_cnt)
{
	int* team = malloc(sizeof(int) * member_cnt);
	for (int i = 0;i < member_cnt;i++) {
		//team[]


	}

	free(team);
}
