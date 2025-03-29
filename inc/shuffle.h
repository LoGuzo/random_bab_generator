#include "../inc/header.h"
#include "../inc/array.h"

#ifndef SHUFFLE_H
#define SHUFFLE_H

void assign_memeber(LPARRAY, LPARRAY, int);
void shuffle_member(LPARRAY);
void swap(LPDATA*, LPDATA*);
int has_group_overlap(LPARRAY, LPARRAY, int);
#endif
