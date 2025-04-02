#include "../inc/header.h"
#include "../inc/array.h"

#ifndef SHUFFLE_H
#define SHUFFLE_H

void assign_memeber(LPARRAY, LPARRAY, int); // 배정 함수
void shuffle_member(LPARRAY); // 셔플 함수
void swap(LPDATA*, LPDATA*); // 스왑 함수
int has_group_overlap(LPARRAY, LPARRAY, int); // 중복 확인 함수
#endif
