#include "../inc/header.h"
#include "../inc/array.h"

#ifndef SHUFFLE_H
#define SHUFFLE_H

void shuffle_member(LPARRAY);
void swap(LPDATA*, LPDATA*);
int makeGroups(LPARRAY students, int groupSize, LPARRAY resultGroups, int* groupCount);
int count_overlap(LPARRAY g1, LPARRAY g2);
int is_group_conflict(LPARRAY group, LPARRAY lastWeekGroups, int lastGroupCount, int groupSize);
int makeGroupsWithConstraint(LPARRAY students, int groupSize,
    LPARRAY lastWeekGroups, int lastGroupCount,
    LPARRAY resultGroups, int* groupCount);

#endif
