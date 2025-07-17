#ifndef ae2f_Ann_Slp_h
#define ae2f_Ann_Slp_h

#include <ae2f/Float.auto.h>

#include <ae2f/Ann/Act.h>

#include <ae2f/errGlob.h>

#include <ae2f/Cast.h>
#include <ae2f/Call.h>

#include <ae2f/Macro.h>
#include <ae2f/Guide.h>


#include <ae2f/Pack/Beg.h>

/**
 * @brief
 * # Single Layered Perceptron
 *
 * Multiple input, multiple output. \n
 * For predicting & training operations are able to be parallel.
 * */
ae2f_structdef(struct, ae2f_AnnSlp_t) {
	/* @brief 
	 * input count
	 */
	size_t m_inc;

	/* @brief 
	 * output count
	 */
	size_t m_outc;
};

ae2f_structdef(struct, ae2f_AnnSlp) {
	/**
	 * @brief
	 * Fields allocated as 2nd array.
	 * */
	ae2f_float_t* m_weight;
	ae2f_float_t* m_bias;
	ae2f_float_t* m_cache;

	ae2f_AnnSlp_t m_Slp[1];

	ae2f_AnnAct_t * m_act, * m_actderiv;
	ae2f_AnnLoss_t* m_lossderiv;

	ae2f_float_t m_learningrate, m_learningrate_bias;

#if ae2f_WhenCXX(!)0
#define ae2f_TMP

	inline static void ae2f_TMP operator delete(void* end);
	inline static void* ae2f_TMP operator new(
			size_t oneonly
			, ae2f_LP(inc * outc) ae2f_float_t* const		weight_opt,
			ae2f_LP(outc) ae2f_float_t* const 		bias_opt,
			ae2f_LP(outc) ae2f_float_t* const 		cache_opt,

			const size_t					inc,
			const size_t					outc,
			const size_t					offset_opt,

			ae2f_opt ae2f_AnnAct_t* const			act,
			ae2f_opt ae2f_AnnAct_t* const			actderiv,
			ae2f_AnnLoss_t* const				lossderiv,
			ae2f_float_t					learningrate,
			ae2f_float_t					learningrate_bias,
			ae2f_opt ae2f_err_t* const			err_opt
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
			ae2f_err_t* const				err
			, ae2f_LP(slp::inc)	const ae2f_float_t*	inp
			, ae2f_LP(slp::outc)	const ae2f_float_t*	out_desired
			);

#undef ae2f_TMP
#endif 
};


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

/**
 * @brief
 * Initiator
 *
 * No allocation is occurred on this function.
 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* _this,
		const size_t inc,
		const size_t outc,
		const size_t offset_opt,
		ae2f_err_t* const err_opt,
		size_t* const initsz_opt
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
		ae2f_LP(inc * outc) ae2f_float_t* const		weight_opt,
		ae2f_LP(outc) ae2f_float_t* const 		bias_opt,
		ae2f_LP(outc) ae2f_float_t* const 		cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,
		ae2f_FREE(ae2f_AnnSlpDel, __ae2f_AnnSlpDel) 
		ae2f_AnnSlp** const 				slp,
		ae2f_opt ae2f_AnnAct_t* const			act,
		ae2f_opt ae2f_AnnAct_t* const			actderiv,
		ae2f_AnnLoss_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* const			err_opt
		) noexcept;

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpDel(
		ae2f_AnnSlp* const slp
		) noexcept;

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpPredict(
		ae2f_err_t* const err_opt
		, const ae2f_AnnSlp* const _this
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc)
		ae2f_float_t* const out
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
		ae2f_err_t* const reterr_opt
		, const ae2f_AnnSlp* const _this
		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in
		, ae2f_LP(_this::outc) 
		const ae2f_float_t* const delta
		) noexcept;

	/** 
	 * @brief
	 * Adjusts the delta based on output and desired output.
	 * */
	ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpFit(
			ae2f_err_t* const reterr_opt
			, const ae2f_AnnSlp* const _this
			, ae2f_LP(_this::inc) const ae2f_float_t* const prm_inp
			, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out
			, ae2f_LP(_this::outc) const ae2f_float_t* const prm_out_desired
			) noexcept;

	/** @brief Calculates the delta based on output and desired output. */
	ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpFetchDelta(
			ae2f_opt ae2f_err_t* const			err
			, const ae2f_AnnSlp*				slp

			, ae2f_LP(slp::outc) const ae2f_float_t* const	out
			, ae2f_LP(slp::outc) const ae2f_float_t* const	out_desired

			, ae2f_LP(slp::outc) ae2f_float_t* const	retdelta
			) noexcept;

	/** @brief Adjusts the weights and biases based on predicted output from input and desired outpu. */
	ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpTrain(
			ae2f_err_t* const				err
			, ae2f_AnnSlp*					slp
			, ae2f_LP(slp::inc)	const ae2f_float_t*	inp
			, ae2f_LP(slp::outc)	const ae2f_float_t*	out_desired
			) noexcept;

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

	ae2f_AnnUtilPtr2(ae2f_float_t, ae2f_AnnSlp);

	ae2f_structdef(struct, ae2f_AnnSlpMk_t) {
		size_t m_stack, m_alloccount;
		ae2f_AnnSlp* m_ptr;
		ae2f_AnnUtilPtr2Name(ae2f_float_t, ae2f_AnnSlp) m_fieldptr;
	};

ae2f_structdef_n(struct, ae2f_AnnSlpPredict_t, ae2f_AnnSlpTrain_t) {
	size_t		m_i, m_j, m_inc, m_outc;
	ae2f_float_t	m_ret;
};

ae2f_structdef(struct, ae2f_AnnSlpFollowSingle_t) {
	size_t		m_j;
};


ae2f_structdef_n(struct, ae2f_AnnSlpFollow_t, ae2f_AnnSlpFit_t, ae2f_AnnSlpFetchDelta_t) {
	size_t		m_i, m_j, m_inc, m_outc;
};

#include <ae2f/Pack/End.h>

#endif

#ifndef ae2f_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Slp.h>
#endif

#define ae2f_Ann_Slp_c

#if !(ae2f_MAC_BUILD) || !(__ae2f_MACRO_GENERATED)
#include <stdlib.h>
#endif /** for non-builds */

#define __ae2f_AnnSlpDel	free
#define __ae2f_AnnSlpDel_C	free

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
		(_this).m_inc	= (inc);
	} else {
		(_this).m_inc	= (incmax_opt);
	}

	(_this).m_outc	=	(outc);
}


ae2f_MAC() _ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* const	_this,
		const size_t		inc,
		const size_t		outc,
		const size_t		offset_opt,
		ae2f_err_t* const	err_opt,
		size_t* const		initsz_opt
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

#define __ae2f_AnnSlpInit_C __ae2f_AnnSlpInit

ae2f_MAC() _ae2f_AnnSlpMk_imp(
		ae2f_AnnSlpMk_t v_mk,

		ae2f_float_t* const weight_opt,
		ae2f_float_t* const bias_opt,
		ae2f_float_t* const cache_opt,

		const size_t inc,
		const size_t outc,
		const size_t offset_opt,


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
					, calloc(v_mk.m_stack + offset_opt, 1)))
	  ) {
		(v_mk).m_alloccount = 0;
		(v_mk).m_fieldptr.b = (v_mk).m_ptr + 1;

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
		ae2f_float_t* const 		bias_opt,
		ae2f_float_t* const 		cache_opt,

		const size_t			inc,
		const size_t			outc,
		const size_t			offset_opt,
		ae2f_AnnSlp** const		slp,

		ae2f_opt ae2f_AnnAct_t* const	act,
		ae2f_opt ae2f_AnnAct_t* const	actderiv,
		ae2f_AnnLoss_t* const		lossderiv,

		ae2f_float_t			learningrate,
		ae2f_float_t			learningrate_bias,

		ae2f_opt ae2f_err_t* const	err_opt
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
				, outc, offset_opt

				, act, actderiv, lossderiv

				, learningrate, learningrate_bias
				);

		(err_opt) && !(*(slp) = v_mk.m_ptr) && 
			(*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	}
}

#define __ae2f_AnnSlpMk_C __ae2f_AnnSlpMk

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
	v_predict.m_inc = (_this).m_inc;
	v_predict.m_outc = (_this).m_outc;

	for(v_predict.m_i = 0; v_predict.m_i < v_predict.m_outc; ++v_predict.m_i)
	{
		v_predict.m_ret = 0;

		for(v_predict.m_j = 0; v_predict.m_j < v_predict.m_inc; ++v_predict.m_j) {
			v_predict.m_ret 
				+= (prm_in)[v_predict.m_j] 
				* ((weight) + v_predict.m_i * v_predict.m_inc)[v_predict.m_j];
		}

		(out)[v_predict.m_i] = (act_opt(
					v_predict.m_ret 
					+ 	(bias)[v_predict.m_i]
					));
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
					v_predict
					, *_this, prm_in, out
					, weight, bias, act_opt
					);
		} else {
			__ae2f_AnnSlpPredict_imp(
					v_predict
					, *_this, prm_in, out
					, weight, bias, 
					);
		}
	}
}

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

ae2f_MAC() _ae2f_AnnSlpFollowSingle_imp(
		ae2f_AnnSlpFollowSingle_t v_follow
		, const ae2f_float_t* prm_in
		, const ae2f_float_t delta
		, ae2f_float_t* const weight
		, const ae2f_float_t learningrate
		, const ae2f_float_t learningrate_bias

		, const size_t inp_sz
		, ae2f_float_t bias
		)
{
	for((v_follow).m_j = 0; (v_follow).m_j < (inp_sz); ++(v_follow).m_j) {
		((weight))[(v_follow).m_j]
			-= (delta) * (prm_in)[(v_follow).m_j] * (learningrate);
	}

	(bias) -= (delta) * (learningrate_bias);
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
	v_follow.m_inc = (_this).m_inc;
	v_follow.m_outc = (_this).m_outc;
	for(v_follow.m_i = 0; v_follow.m_i < v_follow.m_outc; ++v_follow.m_i) {
		__ae2f_AnnSlpFollowSingle_imp(
				v_follow
				, prm_in
				, (delta)[v_follow.m_i]
				, ((weight) + ((v_follow).m_i * (v_follow).m_inc))
				, learningrate, learningrate_bias
				, v_follow.m_inc
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

ae2f_MAC() _ae2f_AnnSlpFetchDelta_imp(
		ae2f_AnnSlpFetchDelta_t v_delta
		, const ae2f_AnnSlp_t		slp

		, const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_AnnAct_t			actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, ae2f_float_t* const		retdelta
		)
{
	v_delta.m_inc = slp.m_inc;
	v_delta.m_outc = slp.m_outc;
	for(v_delta.m_i = 0; v_delta.m_i < v_delta.m_outc; ++v_delta.m_i) 
	{
		(retdelta)[v_delta.m_i]
			= lossderiv(out, out_desired, v_delta.m_i, v_delta.m_outc)
			* actderiv_opt((out)[v_delta.m_i]);
	}
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

ae2f_MAC() _ae2f_AnnSlpFit_imp(
		ae2f_AnnSlpFit_t v_fit

		, const ae2f_AnnSlp_t slp

		, const ae2f_float_t* const inp
		, const ae2f_float_t* const out
		, const ae2f_float_t* const out_desired

		, ae2f_float_t* const weights
		, ae2f_float_t* const bias
		, ae2f_float_t* const cachedelta

		, ae2f_AnnAct_t		actderiv_opt
		, ae2f_AnnLoss_t	lossderiv

		, const ae2f_float_t	learningrate
		, const ae2f_float_t	learningrate_bias
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

		, const ae2f_float_t* const	prm_inp
		, const ae2f_float_t* const	prm_out
		, const ae2f_float_t* const	prm_out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_AnnAct_t	ae2f_opt	actderiv_opt
		, ae2f_AnnLoss_t		lossderiv

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias
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

ae2f_MAC() _ae2f_AnnSlpTrain_imp(
		ae2f_AnnSlpTrain_t		v_train
		, const ae2f_AnnSlp_t		slp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out_cache
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_opt ae2f_AnnAct_t	act
		, ae2f_opt ae2f_AnnAct_t	actderiv
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

#if ae2f_WhenCXX(!)0 && !defined(ae2f_Ann_Slp_cc)
#define ae2f_Ann_Slp_cc

#define ae2f_TMP ae2f_AnnSlp::

inline void ae2f_TMP operator delete(void* end) {
	ae2f_AnnSlpDel(ae2f_reinterpret_cast(ae2f_AnnSlp*, end));
}

inline void* ae2f_TMP operator new(
		size_t oneonly
		, ae2f_LP(inc * outc) ae2f_float_t* const		weight_opt,
		ae2f_LP(outc) ae2f_float_t* const 		bias_opt,
		ae2f_LP(outc) ae2f_float_t* const 		cache_opt,

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
			weight_opt, bias_opt, cache_opt, inc, outc, offset_opt
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
		ae2f_err_t* const				err
		, ae2f_LP(slp::inc)	const ae2f_float_t*	inp
		, ae2f_LP(slp::outc)	const ae2f_float_t*	out_desired
		) 
{
	ae2f_AnnSlpTrain(err, this, inp, out_desired);
}

#undef ae2f_TMP

#endif

#endif
