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
ae2f_structdef(struct, ae2f_AnnSlp) 
#if ae2f_NEED_CLASS
{
	/**
	 * @brief
	 * Weights of the network.
	 */
	ae2f_float_t* restrict m_weight;
	/**
	 * @brief
	 * Biases of the network.
	 */
	ae2f_float_t* restrict m_bias;
	/**
	 * @brief
	 * Cache for layer outputs.
	 */
	ae2f_float_t* restrict m_cache;

	/**
	 * @brief
	 * SLP data.
	 */
	ae2f_AnnSlp_t m_Slp[1];

	/**
	 * @brief
	 * Activation function.
	 */
	ae2f_AnnAct_t * m_act;
	/**
	 * @brief
	 * Derivative of the activation function.
	 */
	ae2f_AnnAct_t * m_actderiv;
	/**
	 * @brief
	 * Derivative of the loss function.
	 */
	ae2f_AnnLoss_t* m_lossderiv;

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
		size_t oneonly
		, ae2f_LP(inc * outc) ae2f_float_t* restrict const	weight_opt,
		ae2f_LP(outc) ae2f_float_t* restrict const 			bias_opt,
		ae2f_LP(outc) ae2f_float_t* restrict const 			cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,

		ae2f_opt ae2f_AnnAct_t* const			act,
		ae2f_opt ae2f_AnnAct_t* const			actderiv,
		ae2f_AnnLoss_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* restrict const		err_opt
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
}
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

#if ae2f_NEED_CLASS

/**
 * @brief
 * Initiator
 *
 * No allocation is occurred on this function.
 */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* restrict _this,
		const size_t inc,
		const size_t outc,
		const size_t offset_opt,
		ae2f_err_t* restrict const err_opt,
		size_t* restrict const initsz_opt
		) noexcept;

/* 
 * @memberof ae2f_AnnSlp
 * @brief 
 * Makes a typical( @ref ae2f_mAnnSp::expected ) perceptron.
 * See @ref ae2f_mAnnSlpInit.
 * 
 * It is heap-allocated. pass the output @ref ae2f_mAnnSpDel after use.
 */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpMk(
		ae2f_LP(inc * outc) ae2f_float_t* restrict const	weight_opt,
		ae2f_LP(outc) ae2f_float_t* restrict const 			bias_opt,
		ae2f_LP(outc) ae2f_float_t* restrict const 			cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,
		const size_t					extra_opt,
		ae2f_FREE(ae2f_AnnSlpDel, __ae2f_AnnSlpDel) 
		ae2f_AnnSlp* restrict * restrict const		slp,
		ae2f_opt ae2f_AnnAct_t* const			act,
		ae2f_opt ae2f_AnnAct_t* const			actderiv,
		ae2f_AnnLoss_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* restrict const			err_opt
		) noexcept;

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpDel(
		ae2f_AnnSlp* restrict const slp
		) noexcept;

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpPredict(
		ae2f_err_t* restrict const err_opt
		, const ae2f_AnnSlp* restrict const _this
		, ae2f_LP(_this::inc)
		const ae2f_float_t* restrict const prm_in
		, ae2f_LP(_this::outc)
		ae2f_float_t* restrict const out
		) noexcept;

	/** 
	 * @brief 
	 * Adjusts the field based on delta
	 *
	 * @param delta
	 *
	 * @param prm_in
	 *
	 * @param field
	 * Contains weights and biases
	 *
	 * @param learningrate
	 * Learning rate for weights
	 *
	 * @param learningrate_bias
	 * Learning rate for biases 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpFollow(
		ae2f_err_t* restrict const reterr_opt
		, const ae2f_AnnSlp* restrict const _this
		, ae2f_LP(_this::inc)
		const ae2f_float_t* restrict const prm_in
		, ae2f_LP(_this::outc) 
		const ae2f_float_t* restrict const delta
		) noexcept;

	/** 
	 * @brief
	 * Adjusts the delta based on output and desired output.
	 * */
		ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpFit(
				ae2f_err_t* const reterr_opt
				, const ae2f_AnnSlp* const _this
				, ae2f_LP(_this::inc) const ae2f_float_t* restrict const prm_inp
				, ae2f_LP(_this::outc) const ae2f_float_t* restrict const prm_out
				, ae2f_LP(_this::outc) const ae2f_float_t* restrict const prm_out_desired
				) noexcept;

	/** @brief Calculates the delta based on output and desired output. */
		ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpFetchDelta(
				ae2f_opt ae2f_err_t* restrict const		err
				, const ae2f_AnnSlp* restrict			slp

				, ae2f_LP(slp::outc) const ae2f_float_t* restrict const	out
				, ae2f_LP(slp::outc) const ae2f_float_t* restrict const	out_desired

				, ae2f_LP(slp::outc) ae2f_float_t* restrict const	retdelta
				) noexcept;

	/** @brief Adjusts the weights and biases based on predicted output from input and desired outpu. */
		ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpTrain(
				ae2f_err_t* const restrict					err
				, ae2f_AnnSlp*	restrict				slp
				, ae2f_LP(slp::inc)	const ae2f_float_t*	restrict	inp
				, ae2f_LP(slp::outc)	const ae2f_float_t*	restrict	out_desired
				) noexcept;

#endif

#if !(ae2f_MAC_BUILD)

#define ae2f_AnnSlpInit		__ae2f_AnnSlpInit_C
#define ae2f_AnnSlpMk		__ae2f_AnnSlpMk_C
#define ae2f_AnnSlpDel		__ae2f_AnnSlpDel_C
#define ae2f_AnnSlpPredict	__ae2f_AnnSlpPredict_C
#define ae2f_AnnSlpFollow	__ae2f_AnnSlpFollow_C
#define ae2f_AnnSlpFit		__ae2f_AnnSlpFit_C
#define ae2f_AnnSlpTrain	__ae2f_AnnSlpTrain_C
#define ae2f_AnnSlpFetchDelta	__ae2f_AnnSlpFetchDelta_C


#endif /** macbuild */
#endif

#if !defined(ae2f_Ann_Slp_imp_h)
#define ae2f_Ann_Slp_imp_h

#include <ae2f/Pack/Beg.h>


ae2f_AnnUtilV2(,ae2f_float_t,*restrict, ,ae2f_AnnSlp, *restrict);

/**
 * @brief
 * Structure for SLP creation data.
 */
ae2f_structdef(struct, ae2f_AnnSlpMk_t) {
	/**
	 * @brief
	 * Stack size and allocation count.
	 */
	size_t m_stack, m_alloccount;
	/**
	 * @brief
	 * Pointer to the created SLP.
	 */
	ae2f_AnnSlp* restrict m_ptr;
	/**
	 * @brief
	 * Field pointer for SLP data.
	 */
	ae2f_AnnUtilV2Name(ae2f_float_t, ae2f_AnnSlp) m_fieldptr;
};

#include <ae2f/Pack/End.h>
#endif

#include "./Slp.auto.h"

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

		ae2f_opt ae2f_AnnAct_t* const			act,
		ae2f_opt ae2f_AnnAct_t* const			actderiv,
		ae2f_AnnLoss_t* const				lossderiv,
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

