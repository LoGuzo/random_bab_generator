#include "../inc/header.h"
#include "../inc/array.h"

#ifndef SHUFFLE_H
#define SHUFFLE_H

void assign_memeber(LPARRAY, LPARRAY, int); // ���� �Լ�
void shuffle_member(LPARRAY); // ���� �Լ�
void swap(LPDATA*, LPDATA*); // ���� �Լ�
int has_group_overlap(LPARRAY, LPARRAY, int); // �ߺ� Ȯ�� �Լ�
#endif
