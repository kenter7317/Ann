#include <ae2fVK/Ann/Mlp.h>
#include <stdio.h>
#include "../vk.h"

#define PRINTSZ(T) printf("sizeof(" #T "): %lu\n", sizeof(T))

int main() {
	/* Test_VkInit(); */

	PRINTSZ(ae2fVK_AnnMlpMk_t);
	PRINTSZ(ae2fVK_AnnMlpMkU0_t);
	PRINTSZ(ae2fVK_AnnMlpMkU1_t);
	PRINTSZ(ae2fVK_AnnMlpMkU2_t);
	PRINTSZ(ae2fVK_AnnMlpMkU3_t);
	PRINTSZ(ae2fVK_AnnMlpMkU4_t);
	PRINTSZ(ae2fVK_AnnMlp);

	/* Test_VkEnd(); */
	return 0;
}
