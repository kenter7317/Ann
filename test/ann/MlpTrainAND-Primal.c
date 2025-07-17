#include <ae2f/Ann/Mlp.h>
#include <stdio.h>
#include <math.h>


static ae2f_float_t Act(ae2f_float_t x) {
	return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t ActDeriv(ae2f_float_t output) {
	return output * (1.0 - output);
}

static ae2f_float_t Backward(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	return (output[i] - target[i]) / c;
}

/** Cross entrophy */
static ae2f_float_t LossDeriv(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	constexpr ae2f_float_t epsilon = 1e-7; // Small value to prevent division by zero
	ae2f_float_t o_i = output[i];
	// Clip output to avoid log(0) or division by zero
	o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
	return (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

const ae2f_float_t inp[4][2] = { 1, 1, 0, 0, 0, 1, 1, 0 };
const ae2f_float_t goal_and[4] = {1, 0, 0, 0 };
const ae2f_float_t goal_or[4] = {1, 0, 1, 1 };

ae2f_float_t output[1] = {0, };

#define MLP_IN			2
#define MLP_OUT_GREATEST	3


ae2f_AnnMlp_t mlp;
ae2f_AnnSlp_t slplast;

const size_t	mlp_szv[] = { 2, 3, 1 };

ae2f_float_t	mlp_weights[2][MLP_OUT_GREATEST][1] = {0, };
ae2f_float_t	mlp_outstream[2][MLP_OUT_GREATEST] = {0, };
ae2f_float_t	mlp_deltastream[2][MLP_OUT_GREATEST] = {0, };
ae2f_float_t	mlp_bias[2][MLP_OUT_GREATEST] = {0, };
ae2f_AnnAct_t*	mlp_acts[2] = { Act, Act };
ae2f_AnnAct_t*	mlp_actderivs[2] = { ActDeriv, ActDeriv };

size_t i, j;

union TEST_STACK {
	ae2f_AnnMlpPredictStream_t	m_predictsteam;
	ae2f_AnnSlpFetchDelta_t		m_fetch;
	ae2f_AnnMlpPropagateAll_t	m_propagate;
} __test_stack;

int main() {
	puts("MlpTrianAND-Primal start");

	puts("Configuring mlp");
	mlp.m_outc = MLP_OUT_GREATEST;
	mlp.m_depth = 3;

	puts("Configuring last slp");
	slplast.m_inc = 3;
	slplast.m_outc = 1;

	for(j = 0; j < 1000; ++j) {
		for(i = 0; i < 4; ++i) {
			/** Predict (with all output cached) */
			__ae2f_AnnMlpPredictStream_imp(
					__test_stack.m_predictsteam
					, mlp
					, inp[i]
					, output
					, mlp_szv
					, &mlp_weights[0][0][0]
					, &mlp_bias[0][0]
					, &mlp_outstream[0][0]
					, mlp_acts
					);

			__ae2f_AnnSlpFetchDelta_imp(
					__test_stack.m_fetch
					, slplast
					, output
					, &goal_and[i]
					, ActDeriv
					, LossDeriv
					, mlp_deltastream[1]
					);

			__ae2f_AnnMlpPropagateAll_imp(
					__test_stack.m_propagate
					, mlp, inp[0], output, mlp_deltastream[1]
					, mlp_szv, mlp_outstream[0], mlp_deltastream[0]
					, mlp_weights[0][0], mlp_bias[0]
					, 0.1, 0.1
					, mlp_actderivs
					);
		}
	}

	for(i = 0; i < 4; ++i) {
		/** Predict (with all output cached) */
		__ae2f_AnnMlpPredictStream_imp(
				__test_stack.m_predictsteam
				, mlp
				, inp[i]
				, output
				, mlp_szv
				, &mlp_weights[0][0][0]
				, &mlp_bias[0][0]
				, &mlp_outstream[0][0]
				, mlp_acts
				);

		printf("Output: %f\n", output[0]);
	}



	return 0;
}
