#include <ae2f/Ann/Conv.h>
#include <stdio.h>
#include <memory.h>

/** @brief SUPER SMALL NUMBER that I would not care when comparing */
#define EPSILON 0.000001

size_t i, j, a, el;
ae2f_err_t e[1] = {0};

size_t test_mMMapFieldIdx(const ae2f_mMMap *const mmap, const size_t dim, const size_t *const idxs) 
{
	size_t ret;
	ae2f_mMMapFieldIdx(mmap, dim, idxs, &ret);
	return ret;
}

uint64_t Conv1dTestNoPad() {
	uint64_t r = 0;

	ae2f_mMMap
		* input
		, * kernel
		, * output;

	size_t inputsz = 10, kernelsz = 3, outsz = 8, stride = 1, padding = 0;

	ae2f_mMMapMk(1, &inputsz, e, &input);
	ae2f_mMMapMk(1, &kernelsz, e, &kernel);
	ae2f_mMMapMk(1, &outsz, e, &output);

	assert(!(*e));

	for(i = 0; i < inputsz; i++) {
		ae2f_mMMapField(input)[i] = i + 1;
	}

	ae2f_mMMapField(kernel)[0] = 0.5;
	ae2f_mMMapField(kernel)[1] = 0.3;
	ae2f_mMMapField(kernel)[2] = 0.2;

	ae2f_AnnCnnConv1d(
			ae2f_mMMapField(input)
			, inputsz
			, ae2f_mMMapField(kernel)
			, kernelsz
			, ae2f_mMMapField(output)
			, NULL, stride
			, padding
			);

	ae2f_float_t 
		matchbuff = 0
		, targets[] = { 
			1.7, 
			2.7, 
			3.7, 
			4.7, 
			5.7, 
			6.7, 
			7.7, 
			8.7
		};
	for(i = 0; i < outsz; i++)
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

	ae2f_mMMapMk(1, &inputsz, e, &input);
	ae2f_mMMapMk(1, &kernelsz, e, &kernel);
	ae2f_mMMapMk(1, &outsz, e, &output);

	assert(!(*e));

	for(i = 0; i < inputsz; i++) {
		ae2f_mMMapField(input)[i] = i + 1;
	}

	ae2f_mMMapField(kernel)[0] = 0.5;
	ae2f_mMMapField(kernel)[1] = 0.3;
	ae2f_mMMapField(kernel)[2] = 0.2;

	// 0, 0, 1, 2, 3, 4, 5, 6, 0, 0
	ae2f_AnnCnnConv1d(
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
	for(i = 0; i < outsz; i++)
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

	size_t 
		inputsz[2] = {4, 4},
		kernelsz[2] = {3, 3}, 
		outsz[2] = {2, 2}, 
		stride[2] = { 1, 1 };

	size_t idxbuff[2] = {0, 0};
	ae2f_float_t kernelelbuff[3] = { 0, };

	ae2f_mMMapMk(2, inputsz, e, &input);
	ae2f_mMMapMk(2, kernelsz, e, &kernel);
	ae2f_mMMapMk(2, outsz, e, &output);
	assert(!(*e));

	puts("obj has made");

	for(i = 0, el = 1; i < 4; i++)
		for(j = 0; j < 4; j++) {
			idxbuff[1] = i;
			idxbuff[0] = j;

			ae2f_mMMapField(input)[test_mMMapFieldIdx(input, 2, idxbuff)]
				= el++;
		}

	puts("field");

	kernelelbuff[0] = 1;
	kernelelbuff[1] = 0;
	kernelelbuff[2] = -1;

	idxbuff[0] = 0;
	idxbuff[1] = 2;


	puts("buff has made");

	memcpy(
			ae2f_mMMapField(kernel)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);

	memcpy(
			ae2f_mMMapField(kernel) + test_mMMapFieldIdx(kernel, 2, idxbuff)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);

	idxbuff[1] = 1;
	kernelelbuff[0] = 2;
	kernelelbuff[1] = 0;
	kernelelbuff[2] = -2;

	memcpy(
			ae2f_mMMapField(kernel) + test_mMMapFieldIdx(kernel, 2, idxbuff)
			, kernelelbuff
			, sizeof(ae2f_float_t) * 3
			);


	for(i = 0, el = 1; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			idxbuff[1] = i;
			idxbuff[0] = j;


			printf("%lu "
					, test_mMMapFieldIdx(
						kernel,
						2, 
						idxbuff
						)
					);
			printf(
					"%f "
					, ae2f_mMMapField(kernel)[test_mMMapFieldIdx(
						kernel, 
						2, 
						idxbuff
						)]
					);
		}
		printf("\n");
	}

	
	puts("__ae2f_AnnConv");
	ae2f_AnnCnnConv(
			2
			, ae2f_mMMapField(input), ae2f_mMMapDimLen(input), 0
			, ae2f_mMMapField(kernel), ae2f_mMMapDimLen(kernel), 0
			, ae2f_mMMapField(output), 0, 0
			, stride, 0
			);

	ae2f_float_t target[2][2] = {
		{-8., -8.}
		, {-8., -8.}
	};

	for(a = 0; a < sizeof(target) / sizeof(**target); a++) {
		printf("Check: %f %f\n", ae2f_mMMapField(output)[a], target[0][a]); 
		if( 
				((ae2f_mMMapField(output)[a] - target[0][a]) 
				 * (ae2f_mMMapField(output)[a] - target[0][a])) 
				> EPSILON 
				)
		{ puts("Match None"); assert("Match None"); }
	}

	ae2f_mMMapDel(input);
	ae2f_mMMapDel(kernel);
	ae2f_mMMapDel(output);

	return 0;
}


uint64_t Pool1dTest()
{
	puts("Pool1dTest");
	ae2f_err_t e = 0;

	/* Define input vector and parameters */
	ae2f_float_t inv[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
	size_t inc = 5;
	size_t window = 2;
	size_t stride = 2;
	ae2f_float_t 
		outv[2] = {0, 0}, /* Output vector */
		outv_expected[4][2] = {
			{2, 4},	/* MAX */
			{1, 3},	/* MIN */
			{3, 7},	/* ADD */
			{1.5, 3.5}	/* AVG */
		};

	size_t outc;          /* Output size */

	/* Array of pooling types and their names for iteration */
	ae2f_eAnnCnnPool types[] = {
		ae2f_eAnnCnnPool_MAX, 
		ae2f_eAnnCnnPool_MIN, 
		ae2f_eAnnCnnPool_ADD, 
		ae2f_eAnnCnnPool_AVG, 
	};
	const char* type_names[] = {
		"MAX", "MIN", "ADD", "AVG", "MIDDLE"
	};
	int num_types = sizeof(types) / sizeof(types[0]);

	/* Test each pooling type */
	for (i = 0; i < num_types; i++) {
		/* Reset output vector to zeros */ 
		outv[0] = 0.0;
		outv[1] = 0.0;

		/* Call the function */
		e = ae2f_AnnCnnPool1d(inv, inc, outv, &outc, window, stride, types[i]);

	    
		/* Check and print results */
		if (e == 0) {
			printf("%s Pooling: outc = %zu, outv = [%f, %f]\n", 
					type_names[i], outc, outv[0], outv[1]);

			if(
					((outv[0] - outv_expected[i][0])
					* (outv[0] - outv_expected[i][0])
					> EPSILON)

					|| ((outv[1] - outv_expected[i][1])
					* (outv[1] - outv_expected[i][1])
					> EPSILON)

					) {
				printf("Mismatch: Expected value: %f, %f\n"
						, outv_expected[i][0]
						, outv_expected[i][1]
						);
				return 8;
			}
		} else {
			printf("%s Pooling: Error occurred: %d\n", type_names[i], e);
			return 8;
		}
	}
	
	return 0;
}

uint64_t Pool2dTest() {
	puts("Pool2dTest START");

    /* Input 4x4 grid, flattened in row-major order */
    ae2f_float_t inv[16] = {
	    1, 2, 3, 4, 
	    5, 6, 7, 8, 
	    9, 10, 11, 12, 
	    13, 14, 15, 16
    };
    
    /* Input dimensions: 4x4 */
    size_t inc[2] = {4, 4};
    
    /* Window size: 2x2 */
    size_t window_opt[2] = {2, 2};
    
    /* Stride: 2x2 */
    size_t stride_opt[2] = {2, 2};
    
    /* Output array (allocate for 2x2 = 4 elements) */
    ae2f_float_t outv[4] = {0, 0, 0, 0};
    
    /* Variable to store computed output size */ 
    size_t outc = 0;
    
    /* Error code */
    ae2f_err_t err = 0;

    /* Test MAX pooling */
    err = ae2f_AnnCnnPool(2, inv, inc, 0, outv, 0, 0, window_opt, 0,  stride_opt, ae2f_eAnnCnnPool_MAX);
    if (err == 0) {
        printf("MAX Pooling: outc = %zu, outv = [%f, %f, %f, %f]\n", 
               outc, outv[0], outv[1], outv[2], outv[3]);


	ae2f_float_t tar[] = {6., 8., 14., 16.};
	for(i = 0; i < sizeof(tar)/ sizeof(tar[0]);i++)
	{
		printf("Match test %lu\n", i);
		if((tar[i] - outv[i]) * (tar[i] - outv[i]) > EPSILON)
		{
			printf("Mismatch: %f, %f\n", tar[i], outv[i]);
			return 16;
		}
	}

    } else {
        printf("MAX Pooling Error: %d\n", err);
	return 16;
    }

    outv[0] = outv[1] = outv[2] = outv[3] = 0;

    /* Test AVG pooling */
    err = ae2f_AnnCnnPool(2, inv, inc, 0, outv, 0, 0, window_opt, 0, stride_opt, ae2f_eAnnCnnPool_AVG);
    if (err == 0) {
        printf("AVG Pooling: outc = %zu, outv = [%f, %f, %f, %f]\n", 
               outc, outv[0], outv[1], outv[2], outv[3]);


	ae2f_float_t tar[] = {3.5, 5.5, 11.5, 13.5};
	for(i = 0; i < sizeof(tar)/ sizeof(tar[0]);i++)
	{
		if((tar[i] - outv[i]) * (tar[i] - outv[i]) > EPSILON)
		{
			printf("Mismatch: %f, %f\n", tar[i], outv[i]);
			return 16;
		}
	}

    } else {
        printf("AVG Pooling Error: %d\n", err);
	return 16;
    }

    return 0;
}

int main() {
	uint64_t 
		a = 
		Conv1dTestNoPad()
		| Conv1dTestWithPad()
		| Conv2DTest()
		| Pool1dTest()
		| Pool2dTest()
		| 0
		;

	printf("Check 0: %lu\n", a);

	return a;
}
