#ifndef ae2f_Ann_Slp_Core_h
#define ae2f_Ann_Slp_Core_h

#include <ae2f/Float.h>
#include <ae2f/errGlob.h>
#include <ae2f/Cast.h>
#include <ae2f/Keys.h>
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
typedef struct {
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
} ae2f_AnnSlp_t, ae2f_AnnSlpREG_t;

/**
 * @brief
 * Structure for SLP prediction and training data.
 */
typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;
} ae2f_AnnSlpPredict_t, ae2f_AnnSlpPredictREG_t;

typedef struct {
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;

} ae2f_AnnSlpTrainREG_t;

typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;

	ae2f_float_t		m_ret;
	ae2f_float_t		m_tmp, m_tmp1;
} ae2f_AnnSlpTrain_t;

typedef struct {
	size_t		m_j;
	ae2f_float_t	m_ret;
	ae2f_float_t	m_tmp;
} ae2f_AnnSlpPredictOne_t;

typedef struct {
	size_t		m_j;
	ae2f_float_t	m_ret;
	ae2f_float_t	m_tmp, m_tmp1;
} ae2f_AnnSlpTrainOne_t;

/**
 * @brief
 * Structure for a single step of the follow operation.
 */
typedef struct {
	/**
	 * @brief
	 * Loop counter.
	 */
	size_t			m_j;
} ae2f_AnnSlpFollowOne_t, ae2f_AnnSlpFollowOneREG_t, ae2f_AnnSlpFitOne_t;

typedef struct ae2f_AnnSlp ae2f_AnnSlp;

/**
 * @brief
 * Structure for SLP creation data.
 */
typedef struct {
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
	union {
		ae2f_float_t* ae2f_restrict	a;
		ae2f_AnnSlp* ae2f_restrict	b;
	} m_fieldptr;
};

typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;
} 		ae2f_AnnSlpFollow_t
, ae2f_AnnSlpFollowREG_t
, ae2f_AnnSlpFitREG_t
, ae2f_AnnSlpFetchDeltaREG_t;

typedef struct
{	ae2f_float_t		m_tmp, m_tmp1;
}
ae2f_AnnSlpFitRAM_t, ae2f_AnnSlpFetchDeltaRAM_t, ae2f_AnnSlpTrainRAM_t;

/**
 * @brief
 * Structure for SLP follow, fit, and fetch delta operations.
 */
typedef struct {
	/**
	 * @brief
	 * Loop counters and layer sizes.
	 */
	size_t			m_i, m_j;
	ae2f_float_t		m_tmp, m_tmp1;
} ae2f_AnnSlpFit_t, ae2f_AnnSlpFetchDelta_t;

#include <ae2f/Pack/End.h>

#endif
