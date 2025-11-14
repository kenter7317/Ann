#ifndef ae2f_NEED_CLASS
#define ae2f_NEED_CLASS	1
#endif


#ifndef ae2f_Ann_Slp_h
#define ae2f_Ann_Slp_h

#include "./Slp.core.h"
#include "./Act.h"

#include <ae2f/Pack/Beg.h>

/**
 * @brief
 * # Single Layered Perceptron
 *
 * This is the main structure for the SLP.
 */
typedef struct ae2f_AnnSlp
#if ae2f_NEED_CLASS
{
	/**
	 * @brief
	 * Weights of the network.
	 */
	ae2f_float_t* ae2f_restrict m_weight;
	/**
	 * @brief
	 * Biases of the network.
	 */
	ae2f_float_t* ae2f_restrict m_bias;
	/**
	 * @brief
	 * Cache for layer outputs.
	 */
	ae2f_float_t* ae2f_restrict m_cachedelta;

	/** @brief cache for activation input */
	ae2f_float_t* ae2f_restrict m_cacheact;

	/**
	 * @brief
	 * SLP data.
	 */
	ae2f_AnnSlpREG_t m_Slp[1];

	/**
	 * @brief
	 * Activation function.
	 */
	ae2f_AnnActFFN_t * m_act;
	/**
	 * @brief
	 * Derivative of the activation function.
	 */
	ae2f_AnnActFFN_t * m_actderiv;
	/**
	 * @brief
	 * Derivative of the loss function.
	 */
	ae2f_AnnLossFFN_t* m_lossderiv;

	/**
	 * @brief
	 * Learning rate for weights.
	 */
	ae2f_float_t m_learningrate;
	/**
	 * @brief
	 * Learning rate for biases.
	 */
	ae2f_float_t m_learningrate_bias;

#if ae2f_WhenCXX(!)0
#define ae2f_TMP

	inline static void ae2f_TMP operator delete(void* end);
	inline static void* ae2f_TMP operator new(
		size_t oneonly,
		ae2f_LP(inc * outc) ae2f_float_t* ae2f_restrict const	weight_opt,
		ae2f_LP(outc) ae2f_float_t* ae2f_restrict const 			bias_opt,
		ae2f_LP(outc) ae2f_float_t* ae2f_restrict const 			cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,

		ae2f_opt ae2f_AnnActFFN_t* const			act,
		ae2f_opt ae2f_AnnActFFN_t* const			actderiv,
		ae2f_AnnLossFFN_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* ae2f_restrict const		err_opt
		) throw();

	inline void ae2f_TMP Predict(
		ae2f_err_t* const err_opt
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc)
		ae2f_float_t* const out
		) const;

	inline void ae2f_TMP Follow(
		ae2f_err_t* const reterr_opt
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc) 
		const ae2f_float_t* const delta
		);

	inline void ae2f_TMP Fit(
		ae2f_err_t* const reterr_opt
		, ae2f_LP(_this::inc) const ae2f_float_t* const prm_inp
		, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out
		, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out_desired
		);

	inline void ae2f_TMP Train(
		ae2f_err_t* const					err
		, ae2f_LP(slp::inc)	const ae2f_float_t*	inp
		, ae2f_LP(slp::outc)	const ae2f_float_t*	out_desired
		);

#undef ae2f_TMP
#endif 
} ae2f_AnnSlp
#endif
;

#include <ae2f/Pack/End.h>


/* @memberof ae2f_mAnnSlp
 * @brief
 * Desired least byte size for initialising.
 *
 * Cache	(output size)
 * Bias		(output size)
 * Weight	(input * output)
 */
#define ae2f_AnnSlpFieldSz(inc, outc) ((((inc) + 2) * ((outc)) * sizeof(ae2f_float_t)) + sizeof(ae2f_AnnSlp))


#define ae2f_AnnSlpInit		__ae2f_AnnSlpInit_C
#define ae2f_AnnSlpMk		__ae2f_AnnSlpMk_C
#define ae2f_AnnSlpDel		__ae2f_AnnSlpDel_C
#define ae2f_AnnSlpPredict	__ae2f_AnnSlpPredict_C
#define ae2f_AnnSlpFollow	__ae2f_AnnSlpFollow_C
#define ae2f_AnnSlpFit		__ae2f_AnnSlpFit_C
#define ae2f_AnnSlpTrain	__ae2f_AnnSlpTrain_C
#define ae2f_AnnSlpFetchDelta	__ae2f_AnnSlpFetchDelta_C

#endif



#include "./Slp.auto.h"

#define __ae2f_AnnSlpMk_imp(v_mk, p_weight_opt, p_bias_opt, p_cache_opt, \
	inc, outc, offset_opt, extra_opt, act, actderiv, lossderiv, lr_w, lr_b)	\
	__ae2f_AnnSlpMkVerbose_imp(						\
			v_mk, p_weight_opt, p_bias_opt, p_cache_opt		\
			, ae2f_reinterpret_cast(ae2f_float_t* const, NULL)	\
			, inc, outc, offset_opt, extra_opt, act, actderiv, lossderiv, lr_w, lr_b)

#if ae2f_WhenCXX(!)0 && !defined(ae2f_Ann_Slp_cc) && ae2f_NEED_CLASS
#define ae2f_Ann_Slp_cc

#define ae2f_TMP ae2f_AnnSlp::

inline void ae2f_TMP operator delete(void* end) {
	ae2f_AnnSlpDel(ae2f_reinterpret_cast(ae2f_AnnSlp*, end));
}

inline void* ae2f_TMP operator new(
		size_t oneonly
		, ae2f_LP(inc * outc) ae2f_float_t* const		weight_opt,
		ae2f_LP(outc) ae2f_float_t* const 			bias_opt,
		ae2f_LP(outc) ae2f_float_t* const 			cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,

		ae2f_opt ae2f_AnnActFFN_t* const			act,
		ae2f_opt ae2f_AnnActFFN_t* const			actderiv,
		ae2f_AnnLossFFN_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* const			err_opt
		) throw() 
{
	if(oneonly != sizeof(ae2f_AnnSlp)) return 0;
	ae2f_AnnSlp* __v[1] = { NULL };

	ae2f_AnnSlpMk(
			weight_opt, bias_opt, cache_opt, inc, outc, offset_opt, 0
			, __v, act, actderiv, lossderiv, learningrate, learningrate_bias, err_opt);

	return __v[0];
}

inline void ae2f_TMP Predict(
		ae2f_err_t* const err_opt
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc)
		ae2f_float_t* const out
		) const 
{
	ae2f_AnnSlpPredict(err_opt, this, prm_in, out);
}

inline void ae2f_TMP Follow(
		ae2f_err_t* const reterr_opt
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc) 
		const ae2f_float_t* const delta
		) 
{
	ae2f_AnnSlpFollow(reterr_opt, this, prm_in, delta);
}

inline void ae2f_TMP Fit(
		ae2f_err_t* const reterr_opt
		, ae2f_LP(_this::inc) const ae2f_float_t* const prm_inp
		, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out
		, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out_desired
		) 
{
	ae2f_AnnSlpFit(reterr_opt, this, prm_inp, prm_out, prm_out_desired);
}

inline void ae2f_TMP Train(
		ae2f_err_t* const					err
		, ae2f_LP(slp::inc)	const ae2f_float_t*	inp
		, ae2f_LP(slp::outc)	const ae2f_float_t*	out_desired
		) 
{
	ae2f_AnnSlpTrain(err, this, inp, out_desired);
}

#undef ae2f_TMP

#endif

