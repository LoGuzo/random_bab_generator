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
        // �̹� �� �� �ϳ� ��������
        int overlap = 0;

        for (int i = 0; i < group_size; i++) {
            int idx = g * group_size + i;
            if (idx >= members->size) break;

            SlackMember* curr = (SlackMember*)members->lpData[idx];

            // ������ ��ü ������ ���� ����� ��� ���� �־����� Ȯ��
            int last_group_index = -1;

            for (int l = 0; l < last_week_members->size; l++) {
                SlackMember* prev = (SlackMember*)last_week_members->lpData[l];
                if (strcmp(curr->name, prev->name) == 0) {
                    last_group_index = l / group_size; // ���� �� �� ��ȣ
                    break;
                }
            }

            // �̹� �� ���� �ٸ� ������ �Բ� ���� �ֿ��� ���� �������� Ȯ��
            for (int j = 0; j < group_size; j++) {
                if (i == j) continue;

                int idx2 = g * group_size + j;
                if (idx2 >= members->size) break;

                SlackMember* other = (SlackMember*)members->lpData[idx2];

                // �� ����� ���� �� ���� ���� �־����� Ȯ��
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

        // 4�� ���� �� �ִ� 6�� �� �� 3�� �̻��̸� 3�� �̻� ��ģ ��
        if (overlap > (group_size + 1 / 2)) {
            return 1; // ���� ����
        }
    }

    return 0; // ���� ����
}
