#ifndef ae2f_Ann_Mlp_core_h
#define ae2f_Ann_Mlp_core_h

#include <ae2f/Keys.h>
#include "./Act.h"
#include <ae2f/Keys.h>

typedef struct ae2f_AnnMlp ae2f_AnnMlp;

/**
 * @brief
 * # Multi Layered Perceptron
 *
 * This is a data-only structure, used for type-punning.
 */
typedef struct {
	/**
	 * @brief
	 * Depth of the network, including input and output layers.
	 */
	size_t	m_depth;

	/**
	 * @brief
	 * Possible greatest output size.
	 */
	size_t	m_outc;

	/** @brief possible greatest weight page size */
	size_t	m_weightc;
} ae2f_AnnMlp_t;


/**
 * @brief
 * Structure used for MLP prediction.
 */
typedef struct {
	/**
	 * @brief
	 * Input, output, and loop counters.
	 */
	size_t		m_inc, m_outc, m_i, m_j, m_k, m_depth, m_outc_max;

	/**
	 * @brief
	 * Return value.
	 */
	ae2f_float_t	m_ret;
} ae2f_AnnMlpPredict_t, ae2f_AnnMlpPredictStream_t;

/**
 * @brief
 * A union for type-punning pointers during MLP initialization.
 * This allows treating a block of memory as different pointer types.
 */
typedef union {
	/**
	 * @brief
	 * Pointer to an MLP structure.
	 */
	ae2f_AnnMlp* ae2f_restrict	m_mlp;
	/**
	 * @brief
	 * Pointer to a size_t array.
	 */
	size_t* ae2f_restrict	m_sz;
	/**
	 * @brief
	 * Pointer to a float array.
	 */
	ae2f_float_t* ae2f_restrict	m_f;
	/**
	 * @brief
	 * Generic void pointer.
	 */
	void* ae2f_restrict		m_void;
	/**
	 * @brief
	 * Generic pointer to a pointer.
	 */
	void** ae2f_restrict		m_ptr;
	/**
	 * @brief
	 * Pointer to an array of activation functions.
	 */
	ae2f_AnnActFFN_t** ae2f_restrict	m_Act;
	/**
	 * @brief
	 * Pointer to an array of loss functions.
	 */
	ae2f_AnnLossFFN_t** ae2f_restrict	m_Loss;

	/** for 1-byte padding */
	char* ae2f_restrict		m_byte;
} ae2f_AnnMlpInitMkPtr_t;

/**
 * @brief
 * Structure for MLP initialization data.
 */
typedef struct {
	/**
	 * @brief
	 * Maximum output count among all layers.
	 */
	size_t m_outc;
	size_t m_weightc;

	/**
	 * @brief
	 * Loop counter.
	 */
	size_t m_i;
} ae2f_AnnMlpInit_t;

/**
 * @brief
 * Structure for MLP creation data.
 */
typedef struct {
	/**
	 * @brief
	 * Maximum output count among all layers.
	 */
	size_t m_outc;

	/** @brief Maximum weight page size */
	size_t m_weightc;

	/**
	 * @brief
	 * Loop counter.
	 */
	size_t m_i;
	/**
	 * @brief
	 * Union for pointer manipulation during creation.
	 */
	ae2f_AnnMlpInitMkPtr_t	m_mkptr;
	/**
	 * @brief
	 * Base pointer to the created MLP.
	 */
	ae2f_AnnMlp* ae2f_restrict	m_mkbase;
} ae2f_AnnMlpMk_t;

/**
 * @brief
 * Structure for calculating a single hidden delta value.
 */
typedef struct {
	/** 
	 * @brief delta seed, not delta.
	 * Use __ae2f_AnnBwd to make delta.
	 * */
	ae2f_float_t	m_ret;
	/**
	 * @brief
	 * Output count of the layer.
	 */
	size_t		m_outc;
	/**
	 * @brief
	 * Loop counter.
	 */
	size_t		m_i;
} ae2f_AnnMlpHidDeltaSingle_t;

/**
 * @brief
 * Stack data for backpropagation.
 * @deprecated This is deprecated.
 */
typedef union {
	size_t m_send;
} ae2f_AnnMlpBwdAllStack_t, ae2f_AnnMlpFollowStack_t;

/**
 * @brief
 * Structure for backpropagation through all layers.
 * @deprecated This is deprecated.
 */
typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t				m_i, m_j, m_inc, m_outc;
	/**
	 * @brief
	 * Layer index.
	 */
	size_t				m_k;
	/**
	 * @brief
	 * Page size for weights.
	 */
	size_t				m_pg_weight;
	/**
	 * @brief
	 * Page size for outputs.
	 */
	size_t				m_pg_out;
	/**
	 * @brief
	 * Stack for propagation.
	 */
	ae2f_AnnMlpBwdAllStack_t	m_stack;
	/**
	 * @brief
	 * Return value.
	 */
	ae2f_float_t				m_ret;
} ae2f_AnnMlpBwdAll_t, ae2f_AnnMlpFollow_t;

/**
 * @brief
 * Structure for MLP training data.
 */
typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t				m_i, m_j, m_inc, m_outc;

	/**
	 * @brief
	 * Layer index.
	 */
	size_t				m_k;

	/**
	 * @brief
	 * Page size for weights.
	 */
	size_t				m_pg_weight;

	/**
	 * @brief
	 * Page size for outputs.
	 */
	size_t				m_pg_out;

	/**
	 * @brief
	 * Maximum output size.
	 */
	size_t				m_outc_max;
	/**
	 * @brief
	 * Network depth.
	 */
	size_t				m_depth;	

	/**
	 * @brief
	 * Stack for propagation.
	 */
	ae2f_AnnMlpBwdAllStack_t	m_stack;

	/**
	 * @brief
	 * Return value.
	 */
	ae2f_float_t				m_ret, m_tmp, m_tmp1;
} ae2f_AnnMlpTrain_t;

#include <ae2f/Pack/End.h>

#endif
