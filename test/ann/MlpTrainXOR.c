#include "ae2f/Float.h"
#include <ae2f/Ann/Mlp.h>
#include <stdio.h>
#include <math.h>

static void Act(ae2f_float_t* r, ae2f_float_t x) {	
	*r = 1.0 / (1.0 + exp(-x));
}

static void ActDeriv(ae2f_float_t* r, ae2f_float_t output) {
	*r =  output * (1.0 - output);
}

static void LossDeriv(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	*r = ((output[i] - target[i]) / c);
}

/** Cross entrophy */
static void
LossDerivCROSS(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	const ae2f_float_t epsilon = 1e-7; // Small value to prevent division by zero
	ae2f_float_t o_i = output[i];
	// Clip output to avoid log(0) or division by zero
	o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
	r[0] = (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

const ae2f_float_t inp[4][2] = {
	{0, 0}, 
	{0, 1}, 
	{1, 0},
	{1, 1}
};

const ae2f_float_t goal_xor[4] = {0, 1, 1, 0};

ae2f_float_t output[1] = { 0.5 };
ae2f_err_t err[1] = {0, };
ae2f_AnnMlp* mlp;
size_t lenv[] = {2, 3, 1};

size_t i, j, k;

int main() {
	ae2f_AnnMlpMk(err, &mlp, 3
			, lenv
			, 0, 0, 0
			, LossDerivCROSS
			, 0, 0, 0, 0
			, 0.6, 0.5

			, 3, 3
			);

	assert(mlp->m_learningrate != 0);
	assert(mlp->m_learningrate_bias != 0);

	for(i = 0; i < 2; i++) {
		mlp->m_bias[i] = ((double)rand() / RAND_MAX) -  0.5;
		mlp->m_act[i] = Act;
		mlp->m_actderiv[i] = ActDeriv;
		for(j = 0; j < 9; j++) {
			mlp->m_weight[j + i * 9] = ((double)rand() / RAND_MAX) - 0.5;
		}
	}

	if(err[0]) {
		printf("[Error]: %d\n", err[0]);
		assert(0 && "errval has occurred.");
	}

	for(j = 0; j < 4; j++) {
		ae2f_AnnMlpPredict(err, mlp, inp[j], output);
		assert(!err[0] && "err from predict");
		printf("%f %f -> %f\n", inp[j][0], inp[j][1], output[0]);
	}

	for(i = 0; i < 9000; i++) {
		for(j = 0; j < 4; j++) {
			ae2f_AnnMlpTrainAuto(
					err, mlp, inp[j]
					, &goal_xor[j]);

			assert(!err[0] && "err from TrainAutoStream");
		}
	}

	for(j = 0; j < 4; j++) {
		ae2f_AnnMlpPredict(err, mlp, inp[j], output);
		assert(!err[0] && "err from predict");
		printf("%f %f -> %f\n", inp[j][0], inp[j][1], output[0]);
	}

	ae2f_AnnMlpDel(mlp);
	return 0;
}
