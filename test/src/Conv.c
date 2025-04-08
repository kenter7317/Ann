#include <ae2f/Ann/Conv.h>
#include <stdio.h>
#include <memory.h>

/** @brief SUPER SMALL NUMBER that I would not care when comparing */
#define EPSILON 0.000001

uint64_t Conv1dTestNoPad() {
	uint64_t r = 0;

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

	__ae2f_AnnConv1d(
			ae2f_mMMapField(input)
			, inputsz

			, ae2f_mMMapField(kernel)
			, kernelsz
			, ae2f_mMMapField(output)
			, NULL, stride, padding
			);

	ae2f_float_t 
		matchbuff = 0
		, targets[] = { 
			1.7, 2.7, 3.7, 4.7, 5.7, 6.7, 7.7, 8.7
		};
	for(size_t i = 0; i < outsz; i++)
	{
		(matchbuff = ae2f_mMMapField(
			output
			, const
			)[i]);

		printf(
				"Match: %f %f\n"
				, matchbuff

				, targets[i]
				);

		if (
				(targets[i] - matchbuff)
				* (targets[i] - matchbuff)

				> EPSILON) {
			puts("Match none");
			r = 1;
		}
	}

	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);
	ae2f_mMMapDel(input);

	return r;
}

uint64_t Conv1dTestWithPad() {
	uint64_t r = 0;

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
	__ae2f_AnnConv1d(
			ae2f_mMMapField(input)
			, inputsz

			, ae2f_mMMapField(kernel)
			, kernelsz
			, ae2f_mMMapField(output)
			, NULL, stride, padding
			);

	ae2f_float_t 
		matchbuff = 0
		, targets[] = {
			0.2, 0.7, 1.7, 2.7, 3.7, 4.7, 4.3, 3
		};
	for(size_t i = 0; i < outsz; i++)
	{
		printf(
				"Match: %f %f\n"
				, (matchbuff = ae2f_mMMapField(
						output
						, const
						)[i])
				, targets[i]
				);

		if (
				(targets[i] - matchbuff)
				* (targets[i] - matchbuff)

				> EPSILON) {
			puts("Match none");
			r = 2;
		}
	}

	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);
	ae2f_mMMapDel(input);

	return r;
}

uint64_t Conv2DTest() {
	puts("Conv2DTest start");

	ae2f_mMMap
		* input
		, * kernel
		, * output;

	uint64_t e = 0;

	size_t 
		inputsz[2] = {4, 4},
		kernelsz[2] = {3, 3}, 
		outsz[2] = {2, 2}, 
		stride[2] = { 1, 1 };

	size_t idxbuff[2] = {0, 0};
	ae2f_float_t kernelelbuff[3] = { 0, };

	input = ae2f_mMMapMk(2, inputsz, 0);
	kernel = ae2f_mMMapMk(2, kernelsz, 0);
	output = ae2f_mMMapMk(2, outsz, 0);

	for(size_t i = 0, el = 1; i < 4; i++)
		for(size_t j = 0; j < 4; j++) {
			idxbuff[1] = i;
			idxbuff[0] = j;

			ae2f_mMMapField(input)[ae2f_mMMapFieldIdx(input, 2, idxbuff)]
				= el++;
		}

	kernelelbuff[0] = 1;
	kernelelbuff[1] = 0;
	kernelelbuff[2] = -1;

	idxbuff[0] = 0;
	idxbuff[1] = 2;

	memcpy(
			ae2f_mMMapField(kernel)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);

	memcpy(
			ae2f_mMMapField(kernel) + ae2f_mMMapFieldIdx(kernel, 2, idxbuff)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);

	idxbuff[1] = 1;
	kernelelbuff[0] = 2;
	kernelelbuff[1] = 0;
	kernelelbuff[2] = -2;

	memcpy(
			ae2f_mMMapField(kernel) + ae2f_mMMapFieldIdx(kernel, 2, idxbuff)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);


	for(size_t i = 0, el = 1; i < 3; i++) {
		for(size_t j = 0; j < 3; j++) {
			idxbuff[1] = i;
			idxbuff[0] = j;


			printf("%d "
					, ae2f_mMMapFieldIdx(
						kernel, 
						2, 
						idxbuff
						)
					);
			printf(
					"%f "
					, ae2f_mMMapField(kernel)[ae2f_mMMapFieldIdx(
						kernel, 
						2, 
						idxbuff
						)]
					);
		}
		printf("\n");
	}

	
	puts("__ae2f_AnnConv");
	__ae2f_AnnConv(
			2
			, ae2f_mMMapField(input), ae2f_mMMapDimLen(input), 0
			, ae2f_mMMapField(kernel), ae2f_mMMapDimLen(kernel), 0
			, ae2f_mMMapField(output), 0
			, stride, 0
			);

	ae2f_float_t target[2][2] = {
		{-8., -8.}
		, {-8., -8.}
	};

	for(size_t a = 0; a < sizeof(target) / sizeof(**target); a++) {
		printf("Check: %f %f\n", ae2f_mMMapField(output)[a], target[0][a]); 
		if( 
				((ae2f_mMMapField(output)[a] - target[0][a]) 
				 * (ae2f_mMMapField(output)[a] - target[0][a])) 
				> EPSILON 
				)
		{ puts("Match None"); e = 4; }
	}

	ae2f_mMMapDel(input);
	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);

	return e;
}

int main() {
	uint64_t 
		a = 
		Conv1dTestNoPad()
		| Conv1dTestWithPad()
		| Conv2DTest()
		| 0
		;

	printf("Check 0: %llu\n", a);

	return a;
}
