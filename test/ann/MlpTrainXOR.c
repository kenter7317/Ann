#include <ae2f/Ann/Mlp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static ae2f_float_t Act(ae2f_float_t x) {	
	return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t ActDeriv(ae2f_float_t output) {
	return output * (1.0 - output);
}

static ae2f_float_t LossDeriv(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	return ((output[i] - target[i]) / c);
}

const ae2f_float_t inp[4][2] = { 
	{0, 0}, 
	{0, 1}, 
	{1, 0},
	{1, 1}
};

const ae2f_float_t goal_xor[4] = {0, 1, 1, 0};

ae2f_float_t output[1] = { 0 };
ae2f_err_t err[1] = {0, };
ae2f_AnnMlp* mlp;
size_t lenv[] = {2, 3, 1};

size_t i, j, k;

int main() {
	ae2f_AnnMlpMk(err, &mlp, 3
			, lenv
			, 0, 0, 0
			, LossDeriv
			, 0, 0, 0, 0
			, 0.6, 0.5
			);

	if(err[0]) {
		printf("[Error]: %d\n", err[0]);
		assert(0 && "errval has occurred.");
	}

	ae2f_AnnMlpDel(mlp);
	return 0;
}
