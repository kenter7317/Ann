#ifndef ae2f_NEED_CLASS
#define ae2f_NEED_CLASS	1
#endif

#ifndef ae2f_Ann_Slp_h
#define ae2f_Ann_Slp_h

#include <ae2f/Float.h>
#include <ae2f/errGlob.h>
#include <ae2f/Cast.h>
#include <ae2f/Call.h>
#include <ae2f/Macro.h>
#include <ae2f/Guide.h>

#if	ae2f_NEED_CLASS || !__ae2f_MACRO_GENERATED
#endif

#include <ae2f/Ann/Act.h>
#include <ae2f/Pack/Beg.h>

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
#include <ae2f/Ann/Util.h>

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

#ifndef ae2f_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Slp.h>
#endif

#define ae2f_Ann_Slp_c

#if !(ae2f_MAC_BUILD) && ae2f_NEED_CLASS || !(__ae2f_MACRO_GENERATED)
#include <stdlib.h>
#include <assert.h>
#endif /** for non-builds */

#define __ae2f_AnnSlpDel	free
#define __ae2f_AnnSlpDel_C	free

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpInitInpSz_imp(
		size_t			v_init
		, ae2f_float_t* const	weight_opt
		, ae2f_float_t* const	bias_opt
		, ae2f_float_t* const	cache_opt
		, const size_t		inc
		, const size_t		outc
		)
{
	(v_init) = 
		sizeof(ae2f_AnnSlp)
		+ 	((weight_opt) ? 0 : (inc) * (outc)) * sizeof(ae2f_float_t)
		+	((bias_opt) ? 0 : (outc)) * sizeof(ae2f_float_t)
		+	((cache_opt) ? 0 : (outc) * sizeof(ae2f_float_t))
		;
}

ae2f_MAC() _ae2f_AnnSlpInit_imp(
		size_t v_init
		, ae2f_AnnSlp_t _this
		, const size_t	inc
		, const size_t	outc
		, const size_t	incmax_opt
		)
{
	/* Calculate maximum input size */
	unless(incmax_opt) {
		__ae2f_AnnSlpInitInpSz_imp(
				v_init, 0, 0, 0, inc, outc
				);
		(_this).m_inc	=	(inc);
	} else {
		(_this).m_inc	=	(incmax_opt);
	}

	(_this).m_outc	=	(outc);
}


ae2f_MAC() _ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* const	_this,
		const size_t		inc,
		const size_t		outc,
		const size_t		offset_opt,
		ae2f_err_t* const		err_opt,
		size_t* const			initsz_opt
		)
{
	if((err_opt) && *(err_opt))
		;
	unless(_this)
		(err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		size_t v_init;
		__ae2f_AnnSlpInit_imp(
				v_init
				, *(_this)
				, inc, outc
				, 0
				);
		(initsz_opt) && (*(initsz_opt) = v_init + offset_opt);
	}
}

#else
#undef	__ae2f_AnnSlpInitInpSz_imp
#define __ae2f_AnnSlpInitInpSz_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#undef	__ae2f_AnnSlpInit_imp
#define __ae2f_AnnSlpInit_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#undef	__ae2f_AnnSlpInit
#define __ae2f_AnnSlpInit(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif



#define __ae2f_AnnSlpInit_C __ae2f_AnnSlpInit

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpMk_imp(
		ae2f_AnnSlpMk_t v_mk,

		ae2f_float_t* const weight_opt,
		ae2f_float_t* const bias_opt,
		ae2f_float_t* const cache_opt,

		const size_t inc,
		const size_t outc,
		const size_t offset_opt,
		const size_t extra_opt,

		ae2f_opt ae2f_AnnAct_t* const act,
		ae2f_opt ae2f_AnnAct_t* const actderiv,
		ae2f_AnnLoss_t* const lossderiv,

		ae2f_float_t learningrate,
		ae2f_float_t learningrate_bias
		)
{
	__ae2f_AnnSlpInitInpSz_imp(v_mk.m_stack, weight_opt, bias_opt, cache_opt, inc, outc);
	if((v_mk.m_ptr = ae2f_reinterpret_cast(
					ae2f_AnnSlp*
					, calloc((v_mk).m_stack + (offset_opt) + (extra_opt), 1)))
	  ) {
		(v_mk).m_alloccount = 0;
		(v_mk).m_fieldptr.b = (v_mk).m_ptr + 1;
		(v_mk).m_fieldptr.b = ae2f_reinterpret_cast(
				ae2f_AnnSlp*
				, ae2f_reinterpret_cast(char*, (v_mk).m_fieldptr.b) + (offset_opt)
				);

		if(weight_opt) {
			(v_mk).m_ptr->m_weight = (weight_opt);
		} else {
			(v_mk).m_ptr->m_weight = ((v_mk).m_fieldptr.a);
			(v_mk).m_fieldptr.a += (inc) * (outc);
		}

		if(bias_opt) {
			(v_mk).m_ptr->m_bias = (bias_opt);
		} else {
			(v_mk).m_ptr->m_bias = (v_mk).m_fieldptr.a;
			(v_mk).m_fieldptr.a += (outc);
		}

		if(cache_opt) {
			(v_mk).m_ptr->m_cache = (cache_opt);
		} else {
			(v_mk).m_ptr->m_cache = (v_mk).m_fieldptr.a;
		}

		__ae2f_AnnSlpInit_imp(
				(v_mk).m_stack
				, v_mk.m_ptr->m_Slp[0]
				, inc
				, outc
				, inc
				);

		(v_mk).m_ptr[0].m_act = act;
		(v_mk).m_ptr[0].m_actderiv = actderiv;
		(v_mk).m_ptr[0].m_lossderiv = lossderiv;
		(v_mk).m_ptr[0].m_learningrate = learningrate;
		(v_mk).m_ptr[0].m_learningrate_bias = learningrate_bias;
	}
}

ae2f_MAC() _ae2f_AnnSlpMk(
		ae2f_float_t* const 		weight_opt,
		ae2f_float_t* const 			bias_opt,
		ae2f_float_t* const 			cache_opt,

		const size_t			inc,
		const size_t			outc,
		const size_t			offset_opt,
		const size_t			extra_opt,
		ae2f_AnnSlp** const		slp,

		ae2f_opt ae2f_AnnAct_t* const	act,
		ae2f_opt ae2f_AnnAct_t* const	actderiv,
		ae2f_AnnLoss_t* const		lossderiv,

		ae2f_float_t			learningrate,
		ae2f_float_t			learningrate_bias,

		ae2f_opt ae2f_err_t* const		err_opt
		)
{
	if((err_opt) && *(err_opt))
		;
	else unless(slp)
		(err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_AnnSlpMk_t v_mk;
		__ae2f_AnnSlpMk_imp(
				v_mk
				, weight_opt, bias_opt, cache_opt
				, inc
				, outc, offset_opt, extra_opt

				, act, actderiv, lossderiv

				, learningrate, learningrate_bias
				);

		(err_opt) && !(*(slp) = v_mk.m_ptr) && 
			(*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	}
}
#else
#undef	__ae2f_AnnSlpMk_imp
#define	__ae2f_AnnSlpMk_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#undef	__ae2f_AnnSlpMk
#define	__ae2f_AnnSlpMk(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif

#define __ae2f_AnnSlpMk_C __ae2f_AnnSlpMk

/** @param v_predict has m_ret. That is the return value. */
ae2f_MAC() _ae2f_AnnSlpPredictOne_imp(
		ae2f_AnnSlpPredictOne_t		v_predict
		, const ae2f_float_t* const	prm_in
		, const ae2f_float_t* const	weight
		, const ae2f_float_t		bias
		, ae2f_AnnAct_t			act_opt
		, const size_t			oidx
		, const size_t isz
		)
{
		(v_predict).m_tmp = 0;

		for((v_predict).m_j = (isz); (v_predict).m_j--; ) {
			(v_predict).m_tmp
				+= (prm_in)[(v_predict).m_j]
				* ((weight) + (oidx) * (isz))[(v_predict).m_j];
		}

		act_opt((&(v_predict).m_ret), ((v_predict).m_tmp + (bias)));
}

ae2f_MAC() _ae2f_AnnSlpPredict_imp(
		ae2f_AnnSlpPredict_t v_predict
		, const ae2f_AnnSlp_t _this
		, const ae2f_float_t* const prm_in
		, ae2f_float_t* const out
		, const ae2f_float_t* const weight
		, const ae2f_float_t* const bias
		, ae2f_AnnAct_t act_opt
		)
{
	for((v_predict).m_i = (_this).m_outc; (v_predict).m_i--; )
	{
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, prm_in
				, weight
				, (bias)[(v_predict).m_i]
				, act_opt
				, (v_predict).m_i
				, (_this).m_inc
				);

		(out)[(v_predict).m_i] = (v_predict).m_ret;
	}
}

ae2f_MAC() _ae2f_AnnSlpPredict(
		ae2f_err_t* const err_opt
		, const ae2f_AnnSlp_t* const _this
		, const ae2f_float_t* const prm_in
		, ae2f_float_t* const out

		, const ae2f_float_t* const weight
		, const ae2f_float_t* const bias
		, ae2f_AnnAct_t act_opt
		)
{
	if((err_opt) && *(err_opt))
		;
	else unless((_this) && (prm_in) && (out) && (weight) && (bias)) 
		(err_opt) && ((err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL));
	else {
		ae2f_AnnSlpPredict_t v_predict;

		if(act_opt) {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, *(_this), (prm_in), out
					, weight, bias, act_opt
					);
		} else {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, *(_this), prm_in, out
					, weight, bias, 
					);
		}
	}
}

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpPredict_C(
		ae2f_err_t* const err_opt
		, const ae2f_AnnSlp* const _this
		, const ae2f_float_t* const prm_in
		, ae2f_float_t* const out
		)
{
	unless(_this)
		(err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		__ae2f_AnnSlpPredict(
				err_opt, (_this)->m_Slp
				, prm_in, out
				, (_this)->m_weight
				, (_this)->m_bias
				, (_this)->m_act
				);
	}
}
#else
#undef	__ae2f_AnnSlpPredict_C
#define	__ae2f_AnnSlpPredict_C(...)  \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif

/** @brief Weights */
ae2f_MAC() _ae2f_AnnSlpFollowOneW_imp(
		const ae2f_float_t inp
		, const ae2f_float_t delta
		, ae2f_float_t* const weight
		, const ae2f_float_t learningrate
		, const size_t inp_sz
		, const size_t inp_idx
		, const size_t out_sz
		, const size_t out_idx
		)
{
	((weight) + (inp_sz) * (out_idx))[(inp_idx)]
		-= (delta) * (inp) * (learningrate);
}

ae2f_MAC() _ae2f_AnnSlpFollowOneB_imp(
		ae2f_float_t r_bias
		, const ae2f_float_t delta
		, const ae2f_float_t learningrate_bias
		) { (r_bias) -= (delta) * (learningrate_bias); }

ae2f_MAC() _ae2f_AnnSlpFollowOne_imp(
		ae2f_AnnSlpFollowOne_t v_follow
		, const ae2f_float_t* prm_in
		, const ae2f_float_t delta
		, ae2f_float_t* const weight
		, const ae2f_float_t learningrate
		, const ae2f_float_t learningrate_bias

		, const size_t inp_sz
		, const size_t out_idx
		, ae2f_float_t v_bias
		)
{
	for((v_follow).m_j = 0; (v_follow).m_j < (inp_sz); ++(v_follow).m_j) {
		((weight) + (inp_sz) * (out_idx))[(v_follow).m_j]
			-= (delta) * (prm_in)[(v_follow).m_j] * (learningrate);
	}

	(v_bias) -= (delta) * (learningrate_bias);
}


ae2f_MAC() _ae2f_AnnSlpFollow_imp(
		ae2f_AnnSlpFollow_t v_follow
		, const ae2f_AnnSlp_t _this
		, const ae2f_float_t* const prm_in
		, const ae2f_float_t* const delta
		, ae2f_float_t* const weight
		, ae2f_float_t* const bias
		, const ae2f_float_t learningrate
		, const ae2f_float_t learningrate_bias
		)
{
	for(v_follow.m_i = (_this).m_outc; (v_follow).m_i--; ) {
		__ae2f_AnnSlpFollowOne_imp(
				v_follow
				, prm_in
				, (delta)[v_follow.m_i]
				, ((weight))
				, learningrate, learningrate_bias
				, (_this).m_inc
				, (v_follow).m_i
				, (bias)[v_follow.m_i]
				);
	}
}

ae2f_MAC() _ae2f_AnnSlpFollow(
		ae2f_err_t* const reterr_opt
		, const ae2f_AnnSlp_t* const _this
		, const ae2f_float_t* const prm_in
		, const ae2f_float_t* const delta
		, ae2f_float_t* const weight
		, ae2f_float_t* const bias
		, const ae2f_float_t learningrate
		, const ae2f_float_t learningrate_bias
		)
{
	if((reterr_opt) && *(reterr_opt))
		;
	else if(learningrate == 0 && learningrate_bias == 0)
		;
	else unless((_this) && (prm_in) && (delta) && (weight) && (bias))
		(reterr_opt) && (*(reterr_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_AnnSlpFollow_t v_follow;
		__ae2f_AnnSlpFollow_imp(
				v_follow
				, *(_this)
				, prm_in
				, delta
				, weight, bias
				, learningrate
				, learningrate_bias
				);
	}
}

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpFollow_C(
		ae2f_err_t* const reterr_opt
		, const ae2f_AnnSlp* const _this
		, const ae2f_float_t* const prm_in
		, const ae2f_float_t* const delta
		) {
	unless((_this))
		(reterr_opt) && (*(reterr_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		__ae2f_AnnSlpFollow(
				reterr_opt, _this->m_Slp
				, prm_in, delta
				, _this->m_weight
				, (_this)->m_bias 
				, (_this)->m_learningrate, (_this)->m_learningrate_bias
				);
	}
}
#else
#undef	__ae2f_AnnSlpFollow_C
#define __ae2f_AnnSlpFollow_C(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif

ae2f_MAC() _ae2f_AnnSlpFetchDeltaOne_imp(
		ae2f_float_t			v_fetchdelta_0,
		ae2f_float_t			v_fetchdelta_1,

		const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, ae2f_float_t			retdelta
		, const size_t			oidx
		, const size_t			osz
		)
{
	actderiv_opt(&(v_fetchdelta_0), (out)[oidx]);
	lossderiv((&(v_fetchdelta_1)), (out), (out_desired), (oidx), (osz));

	(retdelta) = 
		(v_fetchdelta_0) *
		(v_fetchdelta_1)
		;
}

ae2f_MAC() _ae2f_AnnSlpFetchDelta_imp(
		ae2f_AnnSlpFetchDelta_t		v_delta
		, const ae2f_AnnSlp_t		slp

		, const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, ae2f_float_t* const		retdelta
		)
{
	for((v_delta).m_i = (slp).m_outc; (v_delta).m_i--; )
		__ae2f_AnnSlpFetchDeltaOne_imp(
				(v_delta).m_tmp, (v_delta).m_tmp1
				, out, out_desired
				, actderiv_opt, lossderiv
				, (retdelta)[v_delta.m_i]
				, (v_delta).m_i
				, (slp).m_outc
				);
}

ae2f_MAC() _ae2f_AnnSlpFetchDelta(
		ae2f_opt ae2f_err_t* const	err
		, const ae2f_AnnSlp_t* const	slp

		, const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, ae2f_float_t* const		retdelta
		) 
{
	if((err) && *(err))
		;
	else unless((slp) && (out) && (out_desired) && (retdelta))
		(err) && (*(err) |= ae2f_errGlob_PTR_IS_NULL);
	else unless(lossderiv)
		(err) && (*(err) |= ae2f_errGlob_IMP_NOT_FOUND);

	else if(actderiv_opt) {
		ae2f_AnnSlpFetchDelta_t v_delta;
		__ae2f_AnnSlpFetchDelta_imp(
				v_delta, (*(slp))
				, out, out_desired
				, actderiv_opt
				, lossderiv
				, retdelta
				);
	} else {
		ae2f_AnnSlpFetchDelta_t v_delta;
		__ae2f_AnnSlpFetchDelta_imp(
				v_delta, (*(slp))
				, out, out_desired
				, , lossderiv
				, retdelta
				);
	}
}

#if	ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpFetchDelta_C(
		ae2f_opt ae2f_err_t* const	err
		, const ae2f_AnnSlp* const	slp

		, const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t* const		retdelta
		)
{
	if((err) && *(err))
		;
	else unless((slp) && (out) && (out_desired) && (retdelta))
		(err) && (*(err) |= ae2f_errGlob_PTR_IS_NULL);
	else unless((slp)->m_lossderiv)
		(err) && (*(err) |= ae2f_errGlob_IMP_NOT_FOUND);
	else {
		if((slp)->m_actderiv) {
			ae2f_AnnSlpFetchDelta_t v_delta;
			__ae2f_AnnSlpFetchDelta_imp(
					v_delta, (slp)->m_Slp[0]
					, out, out_desired
					, (slp)->m_actderiv, (slp)->m_lossderiv
					, retdelta
					);
		} else {
			ae2f_AnnSlpFetchDelta_t v_delta;
			__ae2f_AnnSlpFetchDelta_imp(
					v_delta, (slp)->m_Slp[0]
					, out, out_desired
					, , (slp)->m_lossderiv
					, retdelta
					);
		}
	}
}
#else
#undef	__ae2f_AnnSlpFetchDelta_C
#define __ae2f_AnnSlpFetchDelta_C(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif

ae2f_MAC() _ae2f_AnnSlpFitOne_imp(
		ae2f_AnnSlpFitOne_t	v_fit

		, const ae2f_float_t* const inp
		, const ae2f_float_t* const out
		, const ae2f_float_t* const out_desired
		, ae2f_float_t* const weight

		, ae2f_float_t r_bias
		, ae2f_float_t r_cachedelta

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, const ae2f_float_t	learningrate
		, const ae2f_float_t	learningrate_bias

		, const size_t iidx
		, const size_t oidx

		, const size_t isz
		, const size_t osz
		)
{
	__ae2f_AnnSlpFetchDeltaOne_imp(
			(v_fit).m_tmp, (v_fit).m_tmp1
			, out, out_desired
			, actderiv_opt, lossderiv
			, r_cachedelta
			, oidx, osz
			);


	__ae2f_AnnSlpFollowOne_imp(
			v_fit, inp, r_cachedelta, weight
			, learningrate, learningrate_bias
			, isz, oidx, r_bias
			);
}

ae2f_MAC() _ae2f_AnnSlpFit_imp(
		ae2f_AnnSlpFit_t v_fit

		, const ae2f_AnnSlp_t slp

		, const ae2f_float_t* const inp
		, const ae2f_float_t* const out
		, const ae2f_float_t* const out_desired

		, ae2f_float_t* const weights
		, ae2f_float_t* const bias
		, ae2f_float_t* const cachedelta

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias
		)
{
	__ae2f_AnnSlpFetchDelta_imp(
			v_fit, slp
			, out, out_desired
			, actderiv_opt, lossderiv
			, cachedelta
			);

	__ae2f_AnnSlpFollow_imp(
			v_fit, slp, inp
			, cachedelta, weights, bias
			, learningrate, learningrate_bias
			);
}

ae2f_MAC() _ae2f_AnnSlpFit(
		ae2f_err_t* const ae2f_opt reterr_opt
		, const ae2f_AnnSlp_t* _this

		, const ae2f_float_t* const		prm_inp
		, const ae2f_float_t* const		prm_out
		, const ae2f_float_t* const		prm_out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_AnnAct_t	ae2f_opt		actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, const ae2f_float_t			learningrate
		, const ae2f_float_t			learningrate_bias
		)
{
	if((reterr_opt) && *(reterr_opt))
		;
	else unless((_this) && (prm_inp) && (prm_out_desired) && (weights) && (bias) && (cachedelta))
		(reterr_opt) && (*(reterr_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else unless(lossderiv)
		(reterr_opt) && (*(reterr_opt) |= ae2f_errGlob_IMP_NOT_FOUND);
	else if ((learningrate) == 0 && (learningrate_bias) == 0)
		;
	else {
		ae2f_AnnSlpFit_t v_fit;
		if(actderiv_opt) {
			__ae2f_AnnSlpFit_imp(
					v_fit
					, (*(_this))
					, prm_inp
					, prm_out
					, prm_out_desired
					, weights
					, bias
					, cachedelta
					, actderiv_opt
					, lossderiv
					, learningrate
					, learningrate_bias
					);
		} else {
			__ae2f_AnnSlpFit_imp(
					v_fit
					, (*(_this))
					, prm_inp
					, prm_out
					, prm_out_desired
					, weights
					, bias
					, cachedelta
					, 
					, lossderiv
					, learningrate
					, learningrate_bias
					);
		}
	}
}

#if	ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpFit_C(
		ae2f_err_t* const reterr_opt
		, const ae2f_AnnSlp* const _this
		, const ae2f_float_t* const prm_inp
		, const ae2f_float_t* const prm_out
		, const ae2f_float_t* const prm_out_desired
		) {
	unless(_this) {
		((reterr_opt) && (*(reterr_opt)|= ae2f_errGlob_PTR_IS_NULL));
	}
	else {
		__ae2f_AnnSlpFit(
				reterr_opt
				, (_this)->m_Slp
				, prm_inp
				, prm_out
				, prm_out_desired
				, (_this)->m_weight
				, (_this)->m_bias
				, (_this)->m_cache
				, (_this)->m_actderiv
				, (_this)->m_lossderiv
				, (_this)->m_learningrate
				, (_this)->m_learningrate_bias
				);
	}
}
#else
#undef	__ae2f_AnnSlpFit_C
#define	__ae2f_AnnSlpFit_C(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]
#endif

ae2f_MAC() _ae2f_AnnSlpTrainOne_imp(
		ae2f_AnnSlpTrainOne_t	v_train

		, ae2f_opt ae2f_AnnAct_t		act
		, ae2f_opt ae2f_AnnAct_t		actderiv
		, ae2f_AnnLoss_t			lossderiv

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out_cache
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t*	const		weight
		, ae2f_float_t			v_bias
		, ae2f_float_t			r_cachedelta

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias

		, const size_t			oidx
		, const size_t			osz
		, const size_t			isz
		)
{
	__ae2f_AnnSlpPredictOne_imp(
			v_train
			, inp
			, weight
			, v_bias
			, act
			, oidx
			, isz
			);

	(out_cache)[oidx] = (v_train).m_ret;

	__ae2f_AnnSlpFitOne_imp(
			v_train
			, inp
			, out_cache, out_desired
			, weight
			, v_bias
			, r_cachedelta
			, actderiv, lossderiv
			, learningrate, learningrate_bias
			, iidx, oidx, isz, osz
			);
}

ae2f_MAC() _ae2f_AnnSlpTrain_imp(
		ae2f_AnnSlpTrain_t			v_train
		, const ae2f_AnnSlp_t		slp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out_cache
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_opt ae2f_AnnAct_t		act
		, ae2f_opt ae2f_AnnAct_t		actderiv
		, ae2f_AnnLoss_t		lossderiv

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias
		)
{
	__ae2f_AnnSlpPredict_imp(v_train, slp, inp, out_cache, weights, bias, act);
	__ae2f_AnnSlpFit_imp(
			v_train, slp, inp, out_cache, out_desired
			, weights, bias, cachedelta
			, actderiv, lossderiv
			, learningrate, learningrate_bias
			);
}

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpTrain_C(
		ae2f_err_t* const	err
		, ae2f_AnnSlp*		slp
		, const ae2f_float_t*	inp
		, const ae2f_float_t*	out_desired
		)
{
	unless(slp)
		(err) && (*(err) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		__ae2f_AnnSlpTrain(
				err, (slp)->m_Slp, inp
				, (slp)->m_cache
				, out_desired
				, (slp)->m_weight
				, (slp)->m_bias
				, (slp)->m_cache
				, (slp)->m_act
				, (slp)->m_actderiv
				, (slp)->m_lossderiv
				, (slp)->m_learningrate
				, (slp)->m_learningrate_bias
				);
	}
}
#else
#undef	__ae2f_AnnSlpTrain_C
#define __ae2f_AnnSlpTrain_C \
	typedef char NO_ae2f_NEED_CLASS[-1]

#endif

ae2f_MAC() _ae2f_AnnSlpTrain(
		ae2f_err_t* const		err
		, const ae2f_AnnSlp_t* const	slp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out_cache
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_opt ae2f_AnnAct_t* const	act
		, ae2f_opt ae2f_AnnAct_t* const	actderiv
		, ae2f_AnnLoss_t* const		lossderiv

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias
		)
{
	if((err) && *(err))
		;
	else unless((slp) && (inp) && (out_cache) && (out_desired) && (weights) && (bias) && (cachedelta))
		(err) && (*(err) |= ae2f_errGlob_PTR_IS_NULL);
	else unless(lossderiv)
		(err) && (*(err) |= ae2f_errGlob_IMP_NOT_FOUND);
	else if((learningrate) == 0 && (learningrate_bias) == 0)
		;
	else  {
		ae2f_AnnSlpTrain_t v_train;
		if (act) {
			if(actderiv) {
				__ae2f_AnnSlpTrain_imp(
						v_train, (*slp), inp, out_cache
						, out_desired, weights, bias
						, cachedelta, act, actderiv, lossderiv
						, learningrate, learningrate_bias
						);
			} else {
				__ae2f_AnnSlpTrain_imp(
						v_train, (*slp), inp, out_cache
						, out_desired, weights, bias
						, cachedelta, act, , lossderiv
						, learningrate, learningrate_bias
						);

			}
		} else {
			if(actderiv) {
				__ae2f_AnnSlpTrain_imp(
						v_train, (*slp), inp, out_cache
						, out_desired, weights, bias
						, cachedelta, , actderiv, lossderiv
						, learningrate, learningrate_bias
						);
			} else {
				__ae2f_AnnSlpTrain_imp(
						v_train, (*slp), inp, out_cache
						, out_desired, weights, bias
						, cachedelta, , , lossderiv
						, learningrate, learningrate_bias
						);

			}
		}
	}
}

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

#endif
