#ifndef ae2f_Ann_Slp_Core_h
#define ae2f_Ann_Slp_Core_h

#include <ae2f/Float.h>
#include <ae2f/errGlob.h>
#include <ae2f/Cast.h>
#include <ae2f/Call.h>
#include <ae2f/Macro.h>
#include <ae2f/Guide.h>

#include <ae2f/Ann/Act.h>
#include <ae2f/Pack/Beg.h>
#include <ae2f/Ann/Util.h>

/**
 * @brief
 * # Single Layered Perceptron
 *
 * Multiple input, multiple output. \n
 * For predicting & training operations are able to be parallel.
 */
ae2f_structdef(struct, ae2f_AnnSlp_t) {
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
ae2f_structdef(struct, ae2f_AnnSlpPredict_t) {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;
	ae2f_float_t		m_tmp;
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
ae2f_structdef_n(struct, ae2f_AnnSlpFollowOne_t, ae2f_AnnSlpFitOne_t) {
	/**
	 * @brief
	 * Loop counter.
	 */
	size_t			m_j;
	ae2f_float_t		m_tmp, m_tmp1;
};


/**
 * @brief
 * Structure for SLP follow, fit, and fetch delta operations.
 */
ae2f_structdef_n(struct, ae2f_AnnSlpFollow_t, ae2f_AnnSlpFit_t, ae2f_AnnSlpFetchDelta_t) {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;
	ae2f_float_t		m_tmp, m_tmp1;
};

#include <ae2f/Pack/End.h>

#endif
