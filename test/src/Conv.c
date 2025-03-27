#include <ae2f/Ann/Conv.h>
#include <stdio.h>

#define EPSILON 0.000001

uint64_t Conv1dTestNoPad() {
	ae2f_mMMap
		* input
		, * kernel
		, * output;

	size_t inputsz = 10, kernelsz = 3, outsz = 8, stride = 1, padding = 0;

	input = ae2f_mMMapMk(1, &inputsz, 0);
	kernel = ae2f_mMMapMk(1, &kernelsz, 0);
	output = ae2f_mMMapMk(1, &outsz, 0);

	for(size_t i = 0; i < inputsz; i++) {
		ae2f_mMMapField(input)[i] = i + 1;
	}

	ae2f_mMMapField(kernel)[0] = 0.5;
	ae2f_mMMapField(kernel)[1] = 0.3;
	ae2f_mMMapField(kernel)[2] = 0.2;

	ae2f_AnnConv1d(input, kernel, output, 0, stride, padding);

	ae2f_float_t 
		matchbuff = 0
		, targets[] = { 
			1.7, 2.7, 3.7, 4.7, 5.7, 6.7, 7.7, 8.7
		};
	for(size_t i = 0; i < outsz; i++)
	{
		printf(
				"Match: %f\n"
				, (matchbuff = ae2f_mMMapField(
						output
						, const
						)[i])
				);

		if (
				(targets[i] - matchbuff)
				* (targets[i] - matchbuff)

				> EPSILON) {
			puts("Match none");
			return 1;
		}
	}

	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);
	ae2f_mMMapDel(input);

	return 0;
}

uint64_t Conv1dTestWithPad() {
	ae2f_mMMap
		* input
		, * kernel
		, * output;

	size_t inputsz = 6, kernelsz = 3, outsz = 8, stride = 1, padding = 2;

	input = ae2f_mMMapMk(1, &inputsz, 0);
	kernel = ae2f_mMMapMk(1, &kernelsz, 0);
	output = ae2f_mMMapMk(1, &outsz, 0);

	for(size_t i = 0; i < inputsz; i++) {
		ae2f_mMMapField(input)[i] = i + 1;
	}

	ae2f_mMMapField(kernel)[0] = 0.5;
	ae2f_mMMapField(kernel)[1] = 0.3;
	ae2f_mMMapField(kernel)[2] = 0.2;

	// 0, 0, 1, 2, 3, 4, 5, 6, 0, 0
	ae2f_AnnConv1d(input, kernel, output, 0, stride, padding);

	ae2f_float_t 
		matchbuff = 0
		, targets[] = { 
			0.2, 0.7, 1.7, 2.7, 3.7, 4.7, 4.3, 3
		};
	for(size_t i = 0; i < outsz; i++)
	{
		printf(
				"Match: %f\n"
				, (matchbuff = ae2f_mMMapField(
						output
						, const
						)[i])
				);

		if (
				(targets[i] - matchbuff)
				* (targets[i] - matchbuff)

				> EPSILON) {
			puts("Match none");
			return 2;
		}
	}

	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);
	ae2f_mMMapDel(input);

	return 0;
}

int main() {
	uint64_t 
		a = 
		Conv1dTestNoPad()
		| Conv1dTestWithPad()
		| 0
		;

	printf("Check 0: %llu\n", a);

	return 0;
}
