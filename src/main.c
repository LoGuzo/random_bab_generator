#include "../inc/array.h"
#include "../inc/header.h"
#include "../inc/shuffle.h"

int main(int argc, char *argv[]) {
	LPARRAY members;
	arrayCreate(&members);
	
	arrayAdd(members, "chk");
	arrayAdd(members, "chk1");
	arrayAdd(members, "chk2");
	arrayAdd(members, "chk3");
	arrayAdd(members, "chk4");
	arrayAdd(members, "chk5");
	arrayAdd(members, "chk6");
	arrayAdd(members, "chk7");
	arrayAdd(members, "chk8");

	shuffle_member(members);

	for (int i = 0;i < members->size;i++) {
		printf("%s", members->lpData[i]);
	}

	arrayDestroy(members);
	return 0;
}
