/**
 * @file Slp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2f_Ann_Slp_h
#define ae2f_Ann_Slp_h

#include <ae2f/Pack/Beg.h>
#include <ae2f/Float.auto.h>

#include <ae2f/Ann/Act.h>

#include <ae2f/errGlob.h>
#include <ae2f/Cast.h>
#include <ae2f/Call.h>

#include <ae2f/Macro.h>
#include <ae2f/Guide.h>

#if !(ae2f_MAC_BUILD) || !(__ae2f_MACRO_GENERATED)
#include <stdlib.h>
#endif

/**
 * @brief
 * # Single Layered Perceptron
 *
 * Multiple input, multiple output. \n
 * For predicting & training operations are able to be parallel.
 * */
ae2f_structdef(struct, ae2f_AnnSlp_t) {
	/* @brief 
	 * output count
	 */
	size_t m_outc;

	/* @brief 
	 * input count
	 */
	size_t m_inc;

#if ae2f_WhenCXX(!)0
#define ae2f_TMP

	inline ae2f_err_t ae2f_TMP Predict(
			const ae2f_float_t* in,
			ae2f_float_t* out_opt
			) const noexcept;

	inline ae2f_err_t ae2f_TMP TrainA(
			const ae2f_float_t* in,
			const ae2f_float_t* delta_optA,
			ae2f_float_t learningrate
			) noexcept;

	inline ae2f_err_t ae2f_TMP TrainB(
			const ae2f_float_t* in,
			const ae2f_float_t* goal_optB,
			ae2f_float_t learningrate
			) noexcept;
#undef ae2f_TMP
#endif
};

ae2f_structdef(struct, ae2f_AnnSlp) {
	/**
	 * @brief
	 * Fields allocated as 2nd array.
	 * */
	ae2f_float_t* m_field;

	ae2f_AnnSlp_t m_Slp[1];
#if ae2f_WhenCXX(!)0
	inline ae2f_TMP ~ae2f_AnnSlp();
	private:
	constexprfun ae2f_AnnSlp() : m_Field(0) m_Slp{0, } {}
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
#define ae2f_AnnSlpFieldSz(inc, outc) (((inc) + 2) * ((outc)) * sizeof(ae2f_float_t))

/** @brief Add this to field and you will get a desired bias position for slp. */
#define __ae2f_AnnSlpBiasPad_Count	(inc, outc) ((inc) * (outc))
#define __ae2f_AnnSlpCache_Count	(inc, outc) (__ae2f_AnnSlpBiasPad_Count(inc, outc) + (outc))


/**
 * @brief
 * Initiator
 *
 * No allocation is occurred on this function.
 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* _this,
		const size_t* incs_optA,
		size_t ginc_optB,
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
		const size_t* const incs_optA,
		const size_t ginc_optB,
		ae2f_float_t* const Field_opt,
		const size_t outc,
		const size_t offset_opt,
		ae2f_AnnSlp** const slp,
		ae2f_err_t* const err_opt
		) noexcept ae2f_FREE(ae2f_AnnSlpDel, __ae2f_AnnSlpDel, free);

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
		, ae2f_AnnAct_t* const act_opt
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
		, const ae2f_float_t learningrate
		, const ae2f_float_t learningrate_bias
		) noexcept;

#define __ae2f_AnnSlpDel	free

#if !(ae2f_MAC_BUILD)

#define ae2f_AnnSlpInit		__ae2f_AnnSlpInit
#define ae2f_AnnSlpMk		__ae2f_AnnSlpMk
#define ae2f_AnnSlpDel		__ae2f_AnnSlpDel
#define ae2f_AnnSlpPredict	__ae2f_AnnSlpPredict
#define ae2f_AnnSlpFollow	__ae2f_AnnSlpFollow

#endif
#endif

#if !defined(ae2f_Ann_Slp_imp_h)
#define ae2f_Ann_Slp_imp_h

#include <ae2f/Pack/Beg.h>

	ae2f_structdef(struct, ae2f_AnnSlpInit_t) {
		size_t m_newinc;
		/** @brief output: desired max input size */
		size_t m_incfin;
		size_t m_i;

		/** @brief output: desired buffer size as byte size */
		size_t m_sz; 

	};
/**
 * @property m_incfin output
 * */
ae2f_structdef(struct, ae2f_AnnSlpInitInpSz_t) {
	size_t m_i, m_incfin, m_newinc, m_sz;
};

ae2f_structdef(union, ae2f_AnnSlpMkStack_t) {
	ae2f_AnnSlpInit_t m_init;
};

ae2f_structdef(struct, ae2f_AnnSlpMk_t) {
	ae2f_AnnSlpMkStack_t m_stack;
	ae2f_AnnSlp* m_ptr;
	ae2f_float_t* m_field;
};

ae2f_structdef(struct, ae2f_AnnSlpPredict_t) {
	size_t		m_i, m_j, m_ic, m_oc;
	ae2f_float_t	m_ret;
};

ae2f_structdef(struct, ae2f_AnnSlpFollowSingle_t) {
	size_t		m_j;
};

ae2f_structdef(struct, ae2f_AnnSlpFollow_t) {
	size_t		m_i, m_j, m_ic, m_oc;
};

/** @brief same */
typedef ae2f_AnnSlpFollow_t ae2f_AnnSlpFit_t;

#include <ae2f/Pack/End.h>

#endif

#ifndef ae2f_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Slp.h>
#endif

#define ae2f_Ann_Slp_c

ae2f_MAC() _ae2f_AnnSlpInitInpSz_imp(
		ae2f_AnnSlpInitInpSz_t v_init
		, const size_t* const incs_optA
		, const size_t ginc_optB
		, ae2f_float_t* const Field_opt
		, const size_t outc
		)
{
	v_init.m_incfin = 0;

	/* Calculate maximum input size */
	for(v_init.m_i = 0; v_init.m_i < (outc); v_init.m_i++) {
		v_init.m_newinc = 
			((incs_optA) ? (incs_optA)[v_init.m_i] : (ginc_optB))	/** inc */
			;

		if(v_init.m_incfin < v_init.m_newinc) 
		{
			v_init.m_incfin = v_init.m_newinc;
		}
	}

	v_init.m_sz = 
		sizeof(ae2f_AnnSlp_t)
		+ 	(Field_opt
				? sizeof(ae2f_float_t) * (v_init.m_incfin + 1) * outc 
				: 0)
		;
}

ae2f_MAC() _ae2f_AnnSlpInit_imp(
		ae2f_AnnSlpInit_t v_init
		, ae2f_AnnSlp_t _this
		, const size_t* const incs_optA
		, const size_t ginc_optB
		, const size_t outc
		, const size_t incmax_opt
		) 
{
	/* Calculate maximum input size */
	unless(incmax_opt) {
		__ae2f_AnnSlpInitInpSz_imp(
				v_init
				, incs_optA, ginc_optB
				, 0, outc);
	}

	(_this).m_inc = (v_init).m_incfin;
	(_this).m_outc = (outc);
}


ae2f_MAC() _ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* const _this,
		const size_t* const incs_optA,
		const size_t ginc_optB,
		const size_t outc,
		const size_t offset_opt,
		ae2f_err_t* const err_opt,
		size_t* const initsz_opt
		) 
{
	if((err_opt) && *(err_opt))
		;
	unless(_this) 
		(err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_AnnSlpInit_t v_init;
		__ae2f_AnnSlpInit_imp(
				v_init
				, *(_this)
				, incs_optA, ginc_optB
				, outc
				, 0
				);
		(initsz_opt) && (*(initsz_opt) = v_init.m_sz + offset_opt);
	}
}

ae2f_MAC() _ae2f_AnnSlpMk_imp(
		ae2f_AnnSlpMk_t v_mk,
		const size_t* const incs_optA,
		const size_t ginc_optB,
		ae2f_float_t* const Field_opt,
		const size_t outc,
		const size_t offset_opt
		)
{
	__ae2f_AnnSlpInitInpSz_imp(v_mk.m_stack.m_init, incs_optA, ginc_optB, Field_opt, outc);
	if((v_mk.m_ptr = calloc(v_mk.m_stack.m_init.m_sz, 1))) {
		v_mk.m_field = Field_opt ? Field_opt : ae2f_reinterpret_cast(ae2f_float_t*, v_mk.m_ptr + 1);
		v_mk.m_ptr->m_field = v_mk.m_field;

		__ae2f_AnnSlpInit_imp(
				v_mk.m_stack.m_init
				, v_mk.m_ptr->m_Slp[0]
				, incs_optA
				, ginc_optB
				, outc
				, v_mk.m_stack.m_init.m_incfin
				);
	}
}

ae2f_MAC() _ae2f_AnnSlpMk(
		const size_t* const incs_optA,
		const size_t ginc_optB,
		ae2f_float_t* const Field_opt,
		const size_t outc,
		const size_t offset_opt,
		ae2f_AnnSlp** const slp,
		ae2f_err_t* const err_opt
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
				, incs_optA, ginc_optB
				, Field_opt
				, outc, offset_opt
				);

		(err_opt) && !(*(slp) = v_mk.m_ptr) && 
			(*(err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	}
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
	v_predict.m_ic = (_this).m_inc;
	v_predict.m_oc = (_this).m_outc;

	for(v_predict.m_i = 0; v_predict.m_i < v_predict.m_oc; ++v_predict.m_i)
	{
		v_predict.m_ret = 0;

		for(v_predict.m_j = 0; v_predict.m_j < v_predict.m_ic; ++v_predict.m_j) {
			v_predict.m_ret 
				+= (prm_in)[v_predict.m_j] 
				* ((weight) + v_predict.m_i * v_predict.m_ic)[v_predict.m_j];
		}

		(out)[v_predict.m_i] = act_opt(
				v_predict.m_ret 
				+ 	(bias)[v_predict.m_i]
				);
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
	else unless((_this) && (prm_in) && (out) && (weight)) 
		(err_opt) && ((err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL));
	else {
		ae2f_AnnSlpPredict_t v_predict;
		__ae2f_AnnSlpPredict_imp(
				v_predict
				, *_this, prm_in, out
				, weight, bias, act_opt
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
		, const size_t out_sz
		, ae2f_float_t bias
		)
{
	for((v_follow).m_j = 0; (v_follow).m_j < (inp_sz); ++(v_follow).m_j) {
		((weight))[(v_follow).m_j]
			-= delta * (prm_in)[(v_follow).m_j] * (learningrate);
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
	v_follow.m_ic = (_this).m_inc;
	v_follow.m_oc = (_this).m_outc;
	for(v_follow.m_i = 0; v_follow.m_i < v_follow.m_oc; ++v_follow.m_i) {
		__ae2f_AnnSlpFollowSingle_imp(
				v_follow
				, prm_in, (delta)[v_follow.m_i]
				, ((weight) + v_follow.m_i * v_follow.m_ic)
				, learningrate, learningrate_bias
				, v_follow.m_ic, v_follow.m_oc
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
	else if(learningrate == 0 || learningrate_bias == 0)
		;
	else unless((_this) && (prm_in) && (delta) && (weight))
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
	v_fit.m_oc = slp.m_outc;
	for(v_fit.m_i = 0; v_fit.m_i < v_fit.m_oc; ++v_fit.m_i) 
	{
		(cachedelta)[v_fit.m_i] 
			= lossderiv(out, out_desired, v_fit.m_i, v_fit.m_ic)
			* actderiv_opt((out)[v_fit.m_i]);
	}

	__ae2f_AnnSlpFollow_imp(
			v_fit, slp, inp
			, cachedelta, weights, bias
			, learningrate, learningrate_bias
			);
}

#endif


#if ae2f_WhenCXX(!)0 && !defined(ae2f_Slp_Ann_cc)
#define ae2f_Slp_Ann_cc

#define ae2f_TMP ae2f_mAnnSlp::

inline ae2f_err_t ae2f_TMP Predict(
		const ae2f_float_t* in,
		ae2f_float_t* out_opt
		) const noexcept {
	return ae2f_mAnnSlpPredict(this, in, out_opt);
}

inline ae2f_err_t ae2f_TMP TrainA(
		const ae2f_float_t* in,
		const ae2f_float_t* delta_optA,
		ae2f_float_t learningrate
		) noexcept {
	return ae2f_mAnnSlpTrainA(this, in, delta_optA, learningrate);
}

inline ae2f_err_t ae2f_TMP TrainB(
		const ae2f_float_t* in,
		const ae2f_float_t* goal_optB,
		ae2f_float_t learningrate
		) noexcept {
	return ae2f_mAnnSlpTrainB(this, in, goal_optB, learningrate);
}

constexprfun ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t i) {
	return ae2f_mAnnSlpPerV(this, i);
}
constexprfun const ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t i) const {
	return ae2f_mAnnSlpPerV(this, i, const);
}

constexprfun size_t* ae2f_TMP PercPad(size_t i) {
	return ae2f_mAnnSlpPerVPad(this)[i];
}
constexprfun const size_t* ae2f_TMP PercPad(size_t i) const {
	return ae2f_mAnnSlpPerVPad(this, const)[i];
}

constexprfun const ae2f_float_t* ae2f_TMP Field() const {
	return ae2f_mAnnSlpField(this, const);
}
constexprfun ae2f_float_t* ae2f_TMP Field() {
	return ae2f_mAnnSlpField(this);
}
constexprfun const ae2f_float_t* ae2f_TMP OutCache() const {
	return ae2f_mAnnSlpOutCache(this, const);
}
constexprfun ae2f_float_t* ae2f_TMP OutCache() {
	return ae2f_mAnnSlpOutCache(this);
}

#undef	ae2f_TMP
#define ae2f_TMP ae2f_AnnSlp::

inline ae2f_TMP ~ae2f_AnnSlp() {
	ae2f_AnnSlpClean(this);
}
#undef ae2f_TMP

#endif
