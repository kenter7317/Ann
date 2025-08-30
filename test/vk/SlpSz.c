#include <ae2fVK/Ann/Slp.h>
#include <stdio.h>
#include "../vk.h"

#define PRINTSZ(T) printf("sizeof(" #T "): %lu\n", sizeof(T))

int main() {
	PRINTSZ(ae2fVK_AnnSlpMk_t);
	PRINTSZ(ae2fVK_AnnSlp);
	return 0;
}
