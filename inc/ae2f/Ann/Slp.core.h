#ifndef ae2f_Ann_Slp_Core_h
#define ae2f_Ann_Slp_Core_h

#include <ae2f/Float.h>
#include <ae2f/errGlob.h>
#include <ae2f/Cast.h>
#include <ae2f/Keys.h>
#include <ae2f/c90/StdBool.h>
#include <ae2f/Guide.h>

#include "./Act.h"
#include "./Util.h"

#include <ae2f/Macro.h>
#include <ae2f/Pack/Beg.h>

/**
 * @brief
 * # Single Layered Perceptron
 *
 * Multiple input, multiple output. \n
 * For predicting & training operations are able to be parallel.
 */
ae2f_structdef_n(struct, ae2f_AnnSlp_t, ae2f_AnnSlpREG_t) {
	/**
	 * @brief 
	 * input count
	 */
	size_t m_inc;

	/**
	 * @brief 
	 * output count
	 */
	size_t m_outc;
};

/**
 * @brief
 * Structure for SLP prediction and training data.
 */
ae2f_structdef_n(struct, ae2f_AnnSlpPredict_t, ae2f_AnnSlpPredictREG_t) {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;
};

ae2f_structdef(struct, ae2f_AnnSlpTrainREG_t) {
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;

};

ae2f_structdef(struct, ae2f_AnnSlpTrain_t) {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;
	ae2f_float_t		m_tmp, m_tmp1;
};

ae2f_structdef(struct, ae2f_AnnSlpPredictOne_t) {
	size_t		m_j;
	ae2f_float_t	m_ret;
	ae2f_float_t	m_tmp;
};

ae2f_structdef(struct, ae2f_AnnSlpTrainOne_t) {
	size_t		m_j;
	ae2f_float_t	m_ret;
	ae2f_float_t	m_tmp, m_tmp1;
};

/**
 * @brief
 * Structure for a single step of the follow operation.
 */
ae2f_structdef_n(struct, ae2f_AnnSlpFollowOne_t, ae2f_AnnSlpFollowOneREG_t, ae2f_AnnSlpFitOne_t) {
	/**
	 * @brief
	 * Loop counter.
	 */
	size_t			m_j;
};


ae2f_structdef(struct, ae2f_AnnSlp);

ae2f_structdef(union, ae2f_AnnSlpMkU0_t) {
	ae2f_float_t* ae2f_restrict	a;
	ae2f_AnnSlp* ae2f_restrict	b;
};

/**
 * @brief
 * Structure for SLP creation data.
 */
ae2f_structdef_n(struct, ae2f_AnnSlpMk_t, ae2f_AnnSlpMkREG_t) {
	/**
	 * @brief
	 * Stack size and allocation count.
	 */
	size_t m_stack, m_alloccount;
	/**
	 * @brief
	 * Pointer to the created SLP.
	 */
	ae2f_AnnSlp* ae2f_restrict m_ptr;
	/**
	 * @brief
	 * Field pointer for SLP data.
	 */
	ae2f_AnnSlpMkU0_t m_fieldptr;
};

ae2f_structdef_n(
		struct
		, ae2f_AnnSlpFollow_t
		, ae2f_AnnSlpFollowREG_t
		, ae2f_AnnSlpFitREG_t
		, ae2f_AnnSlpFetchDeltaREG_t
		)
{
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;
};

ae2f_structdef_n(struct, ae2f_AnnSlpFitRAM_t, ae2f_AnnSlpFetchDeltaRAM_t, ae2f_AnnSlpTrainRAM_t) {
	ae2f_float_t		m_tmp, m_tmp1;
};

/**
 * @brief
 * Structure for SLP follow, fit, and fetch delta operations.
 */
ae2f_structdef_n(struct, ae2f_AnnSlpFit_t, ae2f_AnnSlpFetchDelta_t) {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;
	ae2f_float_t		m_tmp, m_tmp1;
};

#include <ae2f/Pack/End.h>

#endif
