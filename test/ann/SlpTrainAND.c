#include "ae2f/Float.h"
#include "ae2f/errGlob.h"
#include <ae2f/Ann/Slp.h>
#include <stdio.h>
#include <math.h>

static ae2f_AnnAct_t Act, ActDeriv;

static void
Act(ae2f_float_t* r, const ae2f_float_t* x, size_t i, size_t c) {
	r[0] = 1.0 / (1.0 + exp(-x[i]));
}

static void
ActDeriv(ae2f_float_t* r, const ae2f_float_t* _output, size_t i, size_t c) {
	const ae2f_float_t output = _output[i] + 1e-7;
	r[0] = output * (1.0 - output);
}

static ae2f_AnnLoss_t LossDeriv;

/** Cross entrophy */
static void
LossDeriv(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	printf("output: %f, goal: %f\n", output[i], target[i]);

	const ae2f_float_t epsilon = 1e-7; // Small value to prevent division by zero
	ae2f_float_t o_i = output[i];
	// Clip output to avoid log(0) or division by zero
	o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
	r[0] = (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

const ae2f_float_t prm_inp[4][2] = { 1, 1, 0, 0, 0, 1, 1, 0 };
const ae2f_float_t goal_andor[4][2] = {
	{1, 1},
	{0, 0},
	{0, 1},
	{0, 1}
};

ae2f_float_t output[2] = {0, };

ae2f_err_t err = 0;
ae2f_AnnSlp* slp = 0;

size_t i, j;
bool mismatch = 0;

#define THRESHOLD 0.01
#define DIFF_GOOD(a, b) (((a) - (b)) * ((a) - (b))) < THRESHOLD

int tryand() {
	puts("Trial: tryand");
	err = 0;

	do {
		ae2f_AnnSlpMk(
				NULL, NULL, NULL
				, 2
				, 2, 3, 4
				, &slp
				, Act, ActDeriv, LossDeriv
				, 0.1, 0.1
				, &err
			     );

		printf("Learningrate dump: %f, %f\n", slp->m_learningrate, slp->m_learningrate_bias);
		printf("function points: %p %p %p\n", slp->m_act, slp->m_actderiv, slp->m_lossderiv);

		if(err) break;

		for(i = 0 ; i < 2 * 1 + 1; ++i) {
			slp->m_weight[i << 1] = 0.2;
			slp->m_weight[(i << 1) + 1] = 0.2;
		}

		for(i = 0; i < 3000; ++i) {
			for(j = 0; j < sizeof(prm_inp) / sizeof(prm_inp[0]); ++j) {
				ae2f_AnnSlpTrain(&err, slp, prm_inp[j], goal_andor[j]);
				if(err) break;
			}
		}

		for(j = 0; j < sizeof(prm_inp) / sizeof(prm_inp[0]); ++j) {
			ae2f_AnnSlpPredict(&err, slp, prm_inp[j], output);
			if(err) break;

			printf("Got %f, %f\n", *output, output[1]);
			if(DIFF_GOOD(output[0], goal_andor[j][0])) {
				printf("Match. Expected %f\n.", goal_andor[j][0]);
			} else {
				printf("Expected %f. Unexpected.\n", goal_andor[j][0]);
				mismatch = 1;
			}
		}
	} while(0);

	if(mismatch) {
		err |= ae2f_errGlob_NFOUND;
	}

	if(slp) ae2f_AnnSlpDel(slp);
	printf("Code from tryand: %d\n", err);

	return err;
}

int main() {
	return tryand();
}
