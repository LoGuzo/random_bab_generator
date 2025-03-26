#include "shuffle.h"
#include <stdlib.h>
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

void assign_member(char **members, int member_cnt) {
  int *team = malloc(sizeof(int) * member_cnt);
  for (int i = 0; i < member_cnt; i++) {
    // team[]
  }

  free(team);
}
