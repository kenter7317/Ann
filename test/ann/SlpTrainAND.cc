#include "ae2f/Float.h"
#include "ae2f/errGlob.h"
#include <ae2f/Ann/Slp.h>
#include <stdio.h>
#include <math.h>

static void
Act(ae2f_float_t* r, ae2f_float_t x) {
	*r = 1.0 / (1.0 + exp(-x));
}

static void
ActDeriv(ae2f_float_t* r, ae2f_float_t output) {
	output += 1e-7;
	*r = output * (1.0 - output);
}

/** Cross entrophy */
static void
LossDeriv(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	ae2f_float_t epsilon = 1e-7; // Small value to prevent division by zero
	ae2f_float_t o_i = output[i];
	// Clip output to avoid log(0) or division by zero
	o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
	*r = (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

const ae2f_float_t inp[4][2] = { 1, 1, 0, 0, 0, 1, 1, 0 };
const ae2f_float_t goal_and[4] = {1, 0, 0, 0 };
const ae2f_float_t goal_or[4] = {1, 0, 1, 1 };

ae2f_float_t output[1] = {0, };

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

		slp = new (
				NULL, NULL, NULL
				, 2
				, 1, 0
				, Act, ActDeriv, LossDeriv
				, 0.1, 0.1
				, &err
			  ) ae2f_AnnSlp;

		printf("Learningrate dump: %f, %f\n", slp->m_learningrate, slp->m_learningrate_bias);
		printf("function points: %p %p %p\n", slp->m_act, slp->m_actderiv, slp->m_lossderiv);

		if(err) break;

		for(i = 0 ; i < 2 * 1 + 1; ++i) {
			slp->m_weight[i] = 0.2;
		}


		for(i = 0; i < 1000; ++i) {
			for(j = 0; j < sizeof(inp) / sizeof(inp[0]); ++j) {
				slp->Train(&err, inp[j], &goal_and[j]);
				if(err) break;
			}
		}

		for(j = 0; j < sizeof(inp) / sizeof(inp[0]); ++j) {
			slp->Predict(&err, inp[j], output);
			if(err) break;

			printf("Got %f\n", *output);
			if(DIFF_GOOD(output[0], goal_and[j])) {
				printf("Match. Expected %f\n.", goal_and[j]);
			} else {
				printf("Expected %f. Unexpected.\n", goal_and[j]);
				mismatch = 1;
			}
		}
	} while(0);

	if(mismatch) {
		err |= ae2f_errGlob_NFOUND;
	}

	if(slp) delete (slp);
	else puts("slp is null");
	printf("Code from tryand: %d\n", err);

	return err;
}

int main() {
	return tryand();
}
