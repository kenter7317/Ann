#define	ae2f_NEED_CLASS 0

#include <ae2f/Ann/Act.h>
#include <ae2f/Ann/Mlp.h>
#include <ae2f/Ann/Slp.h>

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static void Act(ae2f_float* r, const ae2f_float_t* x, size_t i, size_t c) {	
	*r = 1.0 / (1.0 + exp(-x[i]));
}

static void ActDeriv(ae2f_float_t* r, const ae2f_float_t* output, size_t i, size_t c) {
	*r = output[i] * (1.0 - output[i]);
}

static void LossDeriv(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	*r = ((output[i] - target[i]) / c);
}

const ae2f_float_t prm_inp[4][2] = { 
	{0, 0}, 
	{0, 1}, 
	{1, 0},
	{1, 1}
};

const ae2f_float_t goal_xor[4] = {0, 1, 1, 0};

ae2f_float_t output[1] = { 0 };

#define MLP_DEPTH 3
#define MLP_IN 2
#define MLP_HID 3
#define MLP_OUT 1
#define MLP_OUT_GREATEST MLP_HID // The greatest number of outputs from any single layer

ae2f_AnnMlp_t mlp;
ae2f_AnnSlp_t slplast;

const size_t mlp_szv[] = { MLP_IN, MLP_HID, MLP_OUT };

// --- Corrected Memory Layout ---
// The MLP functions expect a flat, padded memory layout for weights and biases.
// Each layer's weights are expected to start at a fixed stride.
#define MLP_WEIGHT_STRIDE (MLP_OUT_GREATEST * MLP_OUT_GREATEST)
#define MLP_BIAS_STRIDE (MLP_OUT_GREATEST)

ae2f_float_t mlp_weights[(MLP_DEPTH - 1) * MLP_WEIGHT_STRIDE] = {0};
ae2f_float_t mlp_bias[(MLP_DEPTH - 1) * MLP_BIAS_STRIDE] = {0};

ae2f_float_t mlp_outstream[(MLP_DEPTH - 1) * MLP_OUT_GREATEST] = {0};
ae2f_float_t mlp_deltastream[(MLP_DEPTH - 1) * MLP_OUT_GREATEST] = {0};
// --- End Corrected Memory Layout ---

ae2f_AnnAct_t* mlp_acts[MLP_DEPTH - 1] = { Act, Act };
ae2f_AnnAct_t* mlp_actderivs[MLP_DEPTH - 1] = { ActDeriv, ActDeriv };

size_t i, j, k;

union TEST_STACK {
	ae2f_AnnMlpPredictStream_t	m_predictsteam;
	ae2f_AnnSlpFetchDelta_t		m_fetch;
	ae2f_AnnMlpBwdAll_t	m_Bwd;
} __test_stack;

int main() {
	puts("MlpTrainXOR-Primal start");

	puts("Configuring mlp");
	mlp.m_outc = MLP_OUT_GREATEST;
	mlp.m_depth = MLP_DEPTH;

	puts("Configuring last slp");
	slplast.m_inc = MLP_HID;
	slplast.m_outc = MLP_OUT;

	srand(0);

	puts("Initializing weights randomly with correct memory layout");
	// Layer 0: 2 prm_inputs -> 3 neurons
	size_t weight_base_l0 = 0 * MLP_WEIGHT_STRIDE;
	size_t bias_base_l0 = 0 * MLP_BIAS_STRIDE;
	for (i = 0; i < mlp_szv[1]; i++) { // 3 output neurons
		for (k = 0; k < mlp_szv[0]; k++) { // 2 prm_input weights
			mlp_weights[weight_base_l0 + i * mlp_szv[0] + k] = ((double)rand() / RAND_MAX) - 0.5;
		}
		mlp_bias[bias_base_l0 + i] = ((double)rand() / RAND_MAX) - 0.5;
	}

	// Layer 1: 3 prm_inputs -> 1 neuron
	size_t weight_base_l1 = 1 * MLP_WEIGHT_STRIDE;
	size_t bias_base_l1 = 1 * MLP_BIAS_STRIDE;
	for (i = 0; i < mlp_szv[2]; i++) { // 1 output neuron
		for (k = 0; k < mlp_szv[1]; k++) { // 3 prm_input weights
			mlp_weights[weight_base_l1 + i * mlp_szv[1] + k] = ((double)rand() / RAND_MAX) - 0.5;
		}
		mlp_bias[bias_base_l1 + i] = ((double)rand() / RAND_MAX) - 0.5;
	}

	puts("See first output (before training)");
	for(i = 0; i < 4; ++i) {
		__ae2f_AnnMlpPredictStream_imp(
				__test_stack.m_predictsteam, mlp, prm_inp[i], output, mlp_szv,
				mlp_weights, mlp_bias, mlp_outstream, mlp_acts
				);
		printf("Initial Output for [%d, %d]: %f (goal: %d)\n"
				, (int)prm_inp[i][0], (int)prm_inp[i][1], output[0], (int)goal_xor[i]);
	}

	puts("Training...");
	for(j = 0; j < 9000; ++j) {
		for(i = 0; i < 4; ++i) {
			__ae2f_AnnMlpPredictStream_imp(
					__test_stack.m_predictsteam, mlp, prm_inp[i], output, mlp_szv,
					mlp_weights, mlp_bias, mlp_outstream, mlp_acts
					);

			__ae2f_AnnSlpFetchDelta_imp(
					__test_stack.m_fetch, slplast
					, output
					, &goal_xor[i]
					, ActDeriv, LossDeriv, &mlp_deltastream[MLP_OUT_GREATEST] 
					);

			__ae2f_AnnMlpFollow_imp(
					__test_stack.m_Bwd, mlp, prm_inp[i]
					, &mlp_deltastream[MLP_OUT_GREATEST],
					mlp_szv, mlp_outstream, mlp_deltastream,
					mlp_weights, mlp_bias,
					0.6, 0.5,
					mlp_actderivs
					);
		}
	}
	puts("Training complete.");

	puts("See last output after training");
	for(i = 0; i < 4; ++i) {
		__ae2f_AnnMlpPredictStream_imp(
				__test_stack.m_predictsteam, mlp, prm_inp[i], output, mlp_szv,
				mlp_weights, mlp_bias, mlp_outstream, mlp_acts
				);
		printf("Final Output for [%d, %d]: %f (goal: %d)\n"
				, (int)prm_inp[i][0], (int)prm_inp[i][1], output[0], (int)goal_xor[i]
		      );
	}

	return 0;
}

