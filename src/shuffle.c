#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/shuffle.h"
#include "../inc/slack_api.h"

void assign_memeber(LPARRAY member, LPARRAY last_week_member, int group_size)
{
    srand(time(NULL));
    
    do {
        shuffle_member(member);
    } while (has_group_overlap(member, last_week_member, group_size));
}

void shuffle_member(LPARRAY array) {
    for (int i = array->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&array->lpData[i], &array->lpData[j]);
    }
}

void swap(LPDATA* name1, LPDATA* name2) {
    LPDATA temp = *name1;
    *name1 = *name2;
    *name2 = temp;
}

int has_group_overlap(LPARRAY members, LPARRAY last_week_members, int group_size)
{
    int group_count = (members->size + group_size - 1) / group_size;

    for (int g = 0; g < group_count; g++) {
        // 이번 주 조 하나 가져오기
        int overlap = 0;

        for (int i = 0; i < group_size; i++) {
            int idx = g * group_size + i;
            if (idx >= members->size) break;

            SlackMember* curr = (SlackMember*)members->lpData[idx];

            // 지난주 전체 조에서 현재 멤버가 어디 조에 있었는지 확인
            int last_group_index = -1;

            for (int l = 0; l < last_week_members->size; l++) {
                SlackMember* prev = (SlackMember*)last_week_members->lpData[l];
                if (strcmp(curr->name, prev->name) == 0) {
                    last_group_index = l / group_size; // 지난 주 조 번호
                    break;
                }
            }

            // 이번 조 내의 다른 멤버들과 함께 지난 주에도 같은 조였는지 확인
            for (int j = 0; j < group_size; j++) {
                if (i == j) continue;

                int idx2 = g * group_size + j;
                if (idx2 >= members->size) break;

                SlackMember* other = (SlackMember*)members->lpData[idx2];

                // 이 멤버도 지난 주 같은 조에 있었는지 확인
                int other_last_group = -1;
                for (int l = 0; l < last_week_members->size; l++) {
                    SlackMember* prev = (SlackMember*)last_week_members->lpData[l];
                    if (strcmp(other->name, prev->name) == 0) {
                        other_last_group = l / group_size;
                        break;
                    }
                }

                if (last_group_index != -1 && last_group_index == other_last_group) {
                    overlap++;
                }
            }
        }

        // 4명 기준 → 최대 6쌍 비교 → 3쌍 이상이면 3명 이상 겹친 것
        if (overlap > (group_size + 1 / 2)) {
            return 1; // 조건 위반
        }
    }

    return 0; // 조건 만족
}
