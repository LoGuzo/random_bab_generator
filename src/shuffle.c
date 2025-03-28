#include "../inc/shuffle.h"
#include "../inc/slack_api.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void shuffle_member(LPARRAY array) {
    srand(time(NULL));
    for (int i = array->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&array->lpData[i], &array->lpData[j]);
    }
}

void swap(LPDATA* name1, LPDATA* name2) {
    LPDATA* temp = *name1;
    *name1 = *name2;
    *name2 = temp;
}

int makeGroups(LPARRAY students, int groupSize, LPARRAY resultGroups, int* groupCount)
{
    *groupCount = 0;
    shuffle_member(students);

    int i = 0;
    while (i < students->size) {
        LPARRAY group;
        arrayCreate(group); // 새 조 만들기
        for (int j = 0; j < groupSize && i + j < students->size; j++) {
            arrayAdd(group, students->lpData[i + j]);
        }
        resultGroups[*groupCount].lpData = group;
        (*groupCount)++;
        i += groupSize;
    }
    return 1;
}

int count_overlap(LPARRAY g1, LPARRAY g2)
{
    int count = 0;
    for (int i = 0; i < g1->size; i++) {
        for (int j = 0; j < g2->size; j++) {
            if (strcmp(((SlackMember*)g1->lpData[i])->name, ((SlackMember*)g2->lpData[j])->name) == 0) {
                count++;
            }
        }
    }
    return count;
}

int is_group_conflict(LPARRAY group, LPARRAY lastWeekGroups, int lastGroupCount, int groupSize)
{
    for (int i = 0; i < lastGroupCount; i++) {
        if (count_overlap(group, *(lastWeekGroups[i].lpData)) >= groupSize - 2) {
            return 1;
        }
    }
    return 0;
}

int makeGroupsWithConstraint(LPARRAY students, int groupSize, LPARRAY lastWeekGroups, int lastGroupCount, LPARRAY resultGroups, int* groupCount)
{
    const int MAX_ATTEMPTS = 1000;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        int tempCount = 0;
        if (!makeGroups(students, groupSize, resultGroups, &tempCount)) continue;

        int conflict = 0;
        for (int i = 0; i < tempCount; i++) {
            if (is_group_conflict(*(resultGroups[i].lpData), lastWeekGroups, lastGroupCount, groupSize)) {
                conflict = 1;
                break;
            }
        }

        if (!conflict) {
            *groupCount = tempCount;
            return 1; // 성공
        }

        // conflict 시 할당한 그룹 해제
        for (int i = 0; i < tempCount; i++) {
            arrayDestroy(*(resultGroups[i].lpData));
        }
    }

    return 0; // 실패
}
