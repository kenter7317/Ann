#ifndef ae2f_Ann_Slp_c
#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Slp.h>
#endif

#define ae2f_Ann_Slp_c

#include <stdlib.h>
#include <assert.h>

#define __ae2f_AnnSlpDel	free
#define __ae2f_AnnSlpDel_C	free

#if ae2f_NEED_CLASS
ae2f_MAC() _ae2f_AnnSlpInitInpSz_imp(
		size_t		rret_init
		, const bool	prm_weight_opt
		, const bool	prm_bias_opt
		, const bool	prm_cache_opt
		, const bool	prm_cacheact_opt
		, const size_t	prm_inc
		, const size_t	prm_outc
		)
{
	(rret_init) = 
		sizeof(ae2f_AnnSlp)
		+ 	((prm_weight_opt) ? 0 : (prm_inc) * (prm_outc)) * sizeof(ae2f_float_t)
		+	((prm_bias_opt) ? 0 : (prm_outc)) * sizeof(ae2f_float_t)
		+	((prm_cache_opt) ? 0 : (prm_outc) * sizeof(ae2f_float_t))
		+	((prm_cacheact_opt) ? 0 : (prm_outc) * sizeof(ae2f_float_t))
		;
}

ae2f_MAC() _ae2f_AnnSlpInit_imp(
		size_t reg_init
		, ae2f_AnnSlpREG_t rret_this
		, const size_t	prm_inc
		, const size_t	prm_outc
		, const size_t	prm_incmax_opt
		)
{
	/* Calculate maximum input size */
	unless(prm_incmax_opt) {
		__ae2f_AnnSlpInitInpSz_imp(
				reg_init, 0, 0, 0, 0, prm_inc, prm_outc
				);
		(rret_this).m_inc	=	(prm_inc);
	} else {
		(rret_this).m_inc	=	(prm_incmax_opt);
	}

	(rret_this).m_outc	=	(prm_outc);
}


ae2f_MAC(v_reg_init, ) _ae2f_AnnSlpInit_V(
		ae2f_AnnSlp_t* const	pret_slp,
		const size_t		prm_inc,
		const size_t		prm_outc,
		const size_t		prm_offset_opt,
		ae2f_err_t* const	pret_err_opt,
		size_t* const		pret_initsz_opt
		)
{
	if((pret_err_opt) && *(pret_err_opt))
		;
	unless(pret_slp)
		(pret_err_opt) && (*(pret_err_opt) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_reg size_t v_reg_init;
		__ae2f_AnnSlpInit_imp(
				v_reg_init
				, *(pret_slp)
				, prm_inc, prm_outc
				, 0
				);
		(pret_initsz_opt) && (*(pret_initsz_opt) = v_reg_init + prm_offset_opt);
	}
}

ae2f_MAC() _ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* const	pret_slp,
		const size_t		prm_inc,
		const size_t		prm_outc,
		const size_t		prm_offset_opt,
		ae2f_err_t* const	pret_err_opt,
		size_t* const		pret_initsz_opt
		)
{
	__ae2f_AnnSlpInit_V(
			v_reg_init, pret_slp
			, prm_inc, prm_outc
			, prm_offset_opt
			, pret_err_opt
			, pret_initsz_opt
			);
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
ae2f_MAC() _ae2f_AnnSlpMkVerbose_imp(
		ae2f_AnnSlpMk_t reg_mk,

		ae2f_float_t* const prop_weight_opt,
		ae2f_float_t* const prop_bias_opt,
		ae2f_float_t* const prop_cache_opt,
		ae2f_float_t* const prop_cacheact_opt,

		const size_t prm_inc,
		const size_t prm_outc,
		const size_t prm_offset_opt,
		const size_t prm_extra_opt,

		ae2f_opt ae2f_AnnActFFN_t* const fn_act,
		ae2f_opt ae2f_AnnActFFN_t* const fn_actderiv,
		ae2f_AnnLossFFN_t* const fn_lossderiv,

		const ae2f_float_t prm_learningrate,
		const ae2f_float_t prm_learningrate_bias
		)
{
	__ae2f_AnnSlpInitInpSz_imp(
			reg_mk.m_stack
			, prop_weight_opt
			, prop_bias_opt
			, prop_cache_opt
			, prop_cacheact_opt
			, prm_inc, prm_outc
			);
	if((reg_mk.m_ptr = ae2f_reinterpret_cast(
					ae2f_AnnSlp*
					, calloc((reg_mk).m_stack + (prm_offset_opt) + (prm_extra_opt), 1)))
	  ) {
		(reg_mk).m_alloccount = 0;
		(reg_mk).m_fieldptr.b = (reg_mk).m_ptr + 1;
		(reg_mk).m_fieldptr.b = ae2f_reinterpret_cast(
				ae2f_AnnSlp*
				, ae2f_reinterpret_cast(char*, (reg_mk).m_fieldptr.b) + (prm_offset_opt)
				);

		if(prop_weight_opt) {
			(reg_mk).m_ptr->m_weight = (prop_weight_opt);
		} else {
			(reg_mk).m_ptr->m_weight = ((reg_mk).m_fieldptr.a);
			(reg_mk).m_fieldptr.a += (prm_inc) * (prm_outc);
		}

		if(prop_bias_opt) {
			(reg_mk).m_ptr->m_bias = (prop_bias_opt);
		} else {
			(reg_mk).m_ptr->m_bias = (reg_mk).m_fieldptr.a;
			(reg_mk).m_fieldptr.a += (prm_outc);
		}

		if(prop_cache_opt) {
			(reg_mk).m_ptr->m_cachedelta = (prop_cache_opt);
		} else {
			(reg_mk).m_ptr->m_cachedelta = (reg_mk).m_fieldptr.a;
			(reg_mk).m_fieldptr.a += (prm_outc);
		}

		if(prop_cacheact_opt) {
			(reg_mk).m_ptr->m_cacheact = (prop_cacheact_opt);
		} else {
			(reg_mk).m_ptr->m_cacheact = (reg_mk).m_fieldptr.a;
		}

		__ae2f_AnnSlpInit_imp(
				(reg_mk).m_stack
				, reg_mk.m_ptr->m_Slp[0]
				, prm_inc
				, prm_outc
				, prm_inc
				);

		(reg_mk).m_ptr[0].m_act = fn_act;
		(reg_mk).m_ptr[0].m_actderiv = fn_actderiv;
		(reg_mk).m_ptr[0].m_lossderiv = fn_lossderiv;
		(reg_mk).m_ptr[0].m_learningrate = prm_learningrate;
		(reg_mk).m_ptr[0].m_learningrate_bias = prm_learningrate_bias;
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

		ae2f_opt ae2f_AnnActFFN_t* const	act,
		ae2f_opt ae2f_AnnActFFN_t* const	actderiv,
		ae2f_AnnLossFFN_t* const		lossderiv,

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
		ae2f_reg ae2f_AnnSlpMk_t v_mk;
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


ae2f_MAC() _ae2f_AnnSlpPredict_imp(
		ae2f_AnnSlpPredict_t reg_predict
		, const ae2f_AnnSlpREG_t prmreg_slp
		, const ae2f_float_t* const pprm_in
		, ae2f_float_t* const ret
		, ae2f_float_t* const ptr_outcache
		, const ae2f_float_t* const pprm_weight
		, const ae2f_float_t* const pprm_bias
		, ae2f_AnnActFFN_t fn_act
		)
{
	for((reg_predict).m_i = (prmreg_slp).m_outc; (reg_predict).m_i--; )
	{
		(reg_predict).m_ret = 0;

		for((reg_predict).m_j = (prmreg_slp).m_inc; (reg_predict).m_j--; ) {
			(reg_predict).m_ret
				+= (pprm_in)[(reg_predict).m_j]
				* ((pprm_weight) + (reg_predict).m_i * (prmreg_slp).m_inc)[(reg_predict).m_j];
		}

		(ptr_outcache)[(reg_predict).m_i] = (reg_predict).m_ret + (pprm_bias)[(reg_predict).m_i];
	}

	for((reg_predict).m_i = (prmreg_slp).m_outc; (reg_predict).m_i--;) {
		fn_act(&(ret)[(reg_predict).m_i], (ptr_outcache), (reg_predict).m_i, (prmreg_slp).m_outc);
	}
}

ae2f_MAC() _ae2f_AnnSlpPredict(
		ae2f_err_t* const err_opt
		, const ae2f_AnnSlp_t* const _this
		, const ae2f_float_t* const prm_in
		, ae2f_float_t* const out
		, ae2f_float_t* const out_cache

		, const ae2f_float_t* const weight
		, const ae2f_float_t* const bias
		, ae2f_AnnActFFN_t act_opt
		)
{
	if((err_opt) && *(err_opt))
		;
	else unless((_this) && (prm_in) && (out) && (weight) && (bias)) 
		(err_opt) && ((err_opt) && (*(err_opt) |= ae2f_errGlob_PTR_IS_NULL));
	else {
		ae2f_reg ae2f_AnnSlpPredictREG_t v_predict;

		if(act_opt) {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, *(_this), (prm_in), out, out_cache
					, weight, bias, act_opt
					);
		} else {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, *(_this), (prm_in), out, out_cache
					, weight, bias, ae2f_AnnActFFN_PASS 
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
				, (_this)->m_cacheact
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
		ae2f_float_t rret_bias
		, const ae2f_float_t prm_delta
		, const ae2f_float_t prm_learningrate_bias
		) { (rret_bias) -= (prm_delta) * (prm_learningrate_bias); }

ae2f_MAC() _ae2f_AnnSlpFollowOne_imp(
		ae2f_AnnSlpFollowOneREG_t reg_follow
		, const ae2f_float_t* pprm_in
		, const ae2f_float_t pprm_delta
		, ae2f_float_t* const ptr_weight
		, const ae2f_float_t prm_learningrate
		, const ae2f_float_t prm_learningrate_bias

		, const size_t prm_isz
		, const size_t prm_oidx
		, ae2f_float_t rret_bias
		)
{
	for((reg_follow).m_j = (prm_isz); (reg_follow).m_j--; ) {
		((ptr_weight) + (prm_isz) * (prm_oidx))[(reg_follow).m_j]
			-= (pprm_delta) * (pprm_in)[(reg_follow).m_j] * (prm_learningrate);
	}

	(rret_bias) -= (pprm_delta) * (prm_learningrate_bias);
}


ae2f_MAC() _ae2f_AnnSlpFollow_imp(
		ae2f_AnnSlpFollowREG_t reg_follow
		, const ae2f_AnnSlpREG_t prm_slp
		, const ae2f_float_t* const pprm_in
		, const ae2f_float_t* const pprm_delta
		, ae2f_float_t* const ptr_weight
		, ae2f_float_t* const ptr_bias
		, const ae2f_float_t prm_learningrate
		, const ae2f_float_t prm_learningrate_bias
		)
{
	for((reg_follow).m_i = (prm_slp).m_outc; (reg_follow).m_i--; ) {
		__ae2f_AnnSlpFollowOne_imp(
				reg_follow
				, pprm_in
				, (pprm_delta)[(reg_follow).m_i]
				, ((ptr_weight))
				, prm_learningrate, prm_learningrate_bias
				, (prm_slp).m_inc
				, (reg_follow).m_i
				, (ptr_bias)[(reg_follow).m_i]
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
		ae2f_reg ae2f_AnnSlpFollow_t v_follow;
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
		ae2f_float_t		rret,

		ae2f_float_t* const	ptr_tmp0,
		ae2f_float_t* const	ptr_tmp1,

		const ae2f_float_t* const	prm_out,
		const ae2f_float_t* const	prm_out_desired,

		const size_t			prm_oidx,
		const size_t			prm_osz,


		ae2f_AnnActFFN_t			fn_actderiv,
		ae2f_AnnLossFFN_t			fn_lossderiv
		)
{
	fn_actderiv(ptr_tmp0, prm_out, prm_oidx, prm_osz);
	fn_lossderiv(ptr_tmp1, prm_out, prm_out_desired, prm_oidx, prm_osz);

	(rret) = *(ptr_tmp0) * *(ptr_tmp1);
}

ae2f_MAC() _ae2f_AnnSlpFetchDeltaVerbose_imp(
		ae2f_AnnSlpFetchDeltaRAM_t		ram_delta,
		ae2f_AnnSlpFetchDeltaREG_t		reg_delta,
		const ae2f_AnnSlpREG_t			prm_slp,

		const ae2f_float_t* const	pprm_out,
		const ae2f_float_t* const	pprm_out_desired,
		ae2f_AnnActFFN_t		fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		ae2f_float_t* const		pret_delta
		)
{	
	for((reg_delta).m_i = (prm_slp).m_outc; (reg_delta).m_i--; )
	{
		fn_actderiv(
				&(ram_delta).m_tmp1
				, (pprm_out)
				, (reg_delta).m_i
				, (prm_slp).m_outc
				);

		fn_lossderiv(
				&(ram_delta).m_tmp
				, (pprm_out)
				, (pprm_out_desired)
				, (reg_delta).m_i, (prm_slp).m_outc
				);

		(pret_delta)[(reg_delta).m_i]
			= (ram_delta).m_tmp * (ram_delta).m_tmp1;
	}
}

ae2f_MAC() _ae2f_AnnSlpFetchDelta_imp(
		ae2f_AnnSlpFetchDelta_t		tmp_delta,
		const ae2f_AnnSlpREG_t		prm_slp,

		const ae2f_float_t* const	pprm_out,
		const ae2f_float_t* const	pprm_out_desired,
		ae2f_AnnActFFN_t		fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		ae2f_float_t* const		pret_delta
		)
{
	__ae2f_AnnSlpFetchDeltaVerbose_imp(
			tmp_delta, tmp_delta
			, prm_slp
			, pprm_out, pprm_out_desired
			, fn_actderiv, fn_lossderiv
			, pret_delta
			);
}

ae2f_MAC() _ae2f_AnnSlpFetchDelta(
		ae2f_opt ae2f_err_t* const	err
		, const ae2f_AnnSlp_t* const	slp

		, const ae2f_float_t* const	out
		, const ae2f_float_t* const	out_desired

		, ae2f_AnnActFFN_t		actderiv_opt
		, ae2f_AnnLossFFN_t		lossderiv

		, ae2f_float_t* const		retdelta
		) 
{
	if((err) && *(err))
		;
	else unless((slp) && (out) && (out_desired) && (retdelta))
		(err) && (*(err) |= ae2f_errGlob_PTR_IS_NULL);
	else unless(lossderiv)
		(err) && (*(err) |= ae2f_errGlob_IMP_NOT_FOUND);

	else {
		ae2f_AnnSlpFetchDeltaRAM_t		v_ram_delta;
		ae2f_reg ae2f_AnnSlpFetchDeltaREG_t	v_reg_delta;

		if(actderiv_opt) {
			__ae2f_AnnSlpFetchDeltaVerbose_imp(
					v_ram_delta
					, v_reg_delta
					, (*(slp))
					, out, out_desired
					, actderiv_opt
					, lossderiv
					, retdelta
					);
		}
		else {
			__ae2f_AnnSlpFetchDeltaVerbose_imp(
					v_ram_delta
					, v_reg_delta
					, (*(slp))
					, out, out_desired
					, ae2f_AnnActDerivFFN_PASS
					, lossderiv
					, retdelta
					);
		}
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
		ae2f_AnnSlpFetchDeltaRAM_t	v_ram_delta;
		ae2f_AnnSlpFetchDeltaREG_t	v_reg_delta;

		if((slp)->m_actderiv) {
			__ae2f_AnnSlpFetchDeltaVerbose_imp(
					v_ram_delta, v_reg_delta, (slp)->m_Slp[0]
					, out, out_desired
					, (slp)->m_actderiv, (slp)->m_lossderiv
					, retdelta
					);
		} else {
			__ae2f_AnnSlpFetchDeltaVerbose_imp(
					v_ram_delta, v_reg_delta, (slp)->m_Slp[0]
					, out, out_desired
					, ae2f_AnnActDerivFFN_PASS, (slp)->m_lossderiv
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


ae2f_MAC() _ae2f_AnnSlpFitVerbose_imp(
		ae2f_AnnSlpFitRAM_t ram_fit,
		ae2f_AnnSlpFitREG_t reg_fit,

		const ae2f_AnnSlp_t prm_slp,

		const ae2f_float_t* const	pprm_inp,
		const ae2f_float_t* const	pprm_out,
		const ae2f_float_t* const	pprm_out_desired,

		ae2f_float_t* const ptr_weights,
		ae2f_float_t* const ptr_bias,
		ae2f_float_t* const ptr_cachedelta,

		ae2f_AnnActFFN_t		fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		const ae2f_float_t		prm_learningrate,
		const ae2f_float_t		prm_learningrate_bias
		)
{
	__ae2f_AnnSlpFetchDeltaVerbose_imp(
			ram_fit, reg_fit, prm_slp
			, pprm_out, pprm_out_desired
			, fn_actderiv, fn_lossderiv
			, ptr_cachedelta
			);

	__ae2f_AnnSlpFollow_imp(
			reg_fit, prm_slp, pprm_inp
			, ptr_cachedelta, ptr_weights, ptr_bias
			, prm_learningrate, prm_learningrate_bias
			);
}

ae2f_MAC() _ae2f_AnnSlpFit_imp(
		ae2f_AnnSlpFit_t v_fit,

		const ae2f_AnnSlp_t prm_slp,

		const ae2f_float_t* const	pprm_inp,
		const ae2f_float_t* const	pprm_out,
		const ae2f_float_t* const	pprm_out_desired,

		ae2f_float_t* const ptr_weights,
		ae2f_float_t* const ptr_bias,
		ae2f_float_t* const ptr_cachedelta,

		ae2f_AnnActFFN_t		fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		const ae2f_float_t		prm_learningrate,
		const ae2f_float_t		prm_learningrate_bias
		)
{
	__ae2f_AnnSlpFitVerbose_imp(
			v_fit, v_fit
			, prm_slp
			, pprm_inp, pprm_out, pprm_out_desired
			, ptr_weights, ptr_bias, ptr_cachedelta
			, fn_actderiv, fn_lossderiv
			, prm_learningrate, prm_learningrate_bias
			);
}

ae2f_MAC() _ae2f_AnnSlpFit(
		ae2f_err_t* const ae2f_opt reterr_opt
		, const ae2f_AnnSlpREG_t* _this

		, const ae2f_float_t* const		prm_inp
		, const ae2f_float_t* const		prm_out
		, const ae2f_float_t* const		prm_out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_AnnActFFN_t	ae2f_opt		actderiv_opt
		, ae2f_AnnLossFFN_t		lossderiv

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
		ae2f_AnnSlpFitRAM_t		v_ram_fit;
		ae2f_reg ae2f_AnnSlpFitREG_t	v_reg_fit;

		if(actderiv_opt) {
			__ae2f_AnnSlpFitVerbose_imp(
					v_ram_fit
					, v_reg_fit
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
			__ae2f_AnnSlpFitVerbose_imp(
					v_ram_fit
					, v_reg_fit
					, (*(_this))
					, prm_inp
					, prm_out
					, prm_out_desired
					, weights
					, bias
					, cachedelta
					, ae2f_AnnActDerivFFN_PASS
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
				, (_this)->m_cachedelta
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

ae2f_MAC() _ae2f_AnnSlpTrainVerbose_imp(
		ae2f_AnnSlpTrainRAM_t		ram_train,
		ae2f_AnnSlpTrainREG_t		reg_train,
		const ae2f_AnnSlp_t		prm_slp,

		const ae2f_float_t* const	pprm_inp,
		ae2f_float_t* const		pret_out,
		ae2f_float_t* const		ptr_out_cache,
		const ae2f_float_t* const	pprm_out_desired,

		ae2f_float_t* const		ptr_weights,
		ae2f_float_t* const		ptr_bias,
		ae2f_float_t* const 		ptr_cachedelta,

		ae2f_opt ae2f_AnnActFFN_t	fn_act,
		ae2f_opt ae2f_AnnActFFN_t	fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		const ae2f_float_t		prm_learningrate,
		const ae2f_float_t		prm_learningrate_bias
		)
{
	__ae2f_AnnSlpPredict_imp(
			reg_train
			, prm_slp
			, pprm_inp
			, pret_out
			, ptr_out_cache
			, ptr_weights
			, ptr_bias
			, fn_act
			);

	__ae2f_AnnSlpFitVerbose_imp(
			ram_train
			, reg_train
			, prm_slp
			, pprm_inp
			, pret_out
			, pprm_out_desired
			, ptr_weights, ptr_bias, ptr_cachedelta
			, fn_actderiv, fn_lossderiv
			, prm_learningrate, prm_learningrate_bias
			);
}

ae2f_MAC() _ae2f_AnnSlpTrain_imp(
		ae2f_AnnSlpTrain_t		tmp_train,
		const ae2f_AnnSlp_t		prm_slp,

		const ae2f_float_t* const	pprm_inp,
		ae2f_float_t* const		pret_out,
		ae2f_float_t* const		ptr_out_cache,
		const ae2f_float_t* const	pprm_out_desired,

		ae2f_float_t* const		ptr_weights,
		ae2f_float_t* const		ptr_bias,
		ae2f_float_t* const 		ptr_cachedelta,

		ae2f_opt ae2f_AnnActFFN_t	fn_act,
		ae2f_opt ae2f_AnnActFFN_t	fn_actderiv,
		ae2f_AnnLossFFN_t		fn_lossderiv,

		const ae2f_float_t		prm_learningrate,
		const ae2f_float_t		prm_learningrate_bias
		)
{
	__ae2f_AnnSlpTrainVerbose_imp(
			tmp_train, tmp_train
			, prm_slp
			, pprm_inp, pret_out
			, ptr_out_cache, pprm_out_desired
			, ptr_weights, ptr_bias, ptr_cachedelta
			, fn_act, fn_actderiv, fn_lossderiv
			, prm_learningrate, prm_learningrate_bias
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
				, (slp)->m_cacheact
				, out_desired
				, (slp)->m_weight
				, (slp)->m_bias
				, (slp)->m_cachedelta
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
		, const ae2f_AnnSlpREG_t* const	slp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out_cache
		, const ae2f_float_t* const	out_desired

		, ae2f_float_t* const		weights
		, ae2f_float_t* const		bias
		, ae2f_float_t* const 		cachedelta

		, ae2f_opt ae2f_AnnActFFN_t* const	act
		, ae2f_opt ae2f_AnnActFFN_t* const	actderiv
		, ae2f_AnnLossFFN_t* const		lossderiv

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
		ae2f_reg ae2f_AnnSlpTrainREG_t v_reg_train;
		ae2f_AnnSlpTrainRAM_t v_ram_train;

		if (act) {
			if(actderiv) {
				__ae2f_AnnSlpTrainVerbose_imp(
						v_ram_train, v_reg_train
						, (*slp), inp, out_cache, out_cache
						, out_desired, weights, bias
						, cachedelta, act, actderiv, lossderiv
						, learningrate, learningrate_bias
						);
			} else {
				__ae2f_AnnSlpTrainVerbose_imp(
						v_ram_train, v_reg_train
						, (*slp), inp, out_cache, out_cache
						, out_desired, weights, bias
						, cachedelta, act, ae2f_AnnActDerivFFN_PASS, lossderiv
						, learningrate, learningrate_bias
						);

			}
		} else {
			if(actderiv) {
				__ae2f_AnnSlpTrainVerbose_imp(
						v_ram_train, v_reg_train
						, (*slp), inp, out_cache, out_cache
						, out_desired, weights, bias
						, cachedelta, ae2f_AnnActFFN_PASS, actderiv, lossderiv
						, learningrate, learningrate_bias
						);
			} else {
				__ae2f_AnnSlpTrainVerbose_imp(
						v_ram_train, v_reg_train
						, (*slp), inp, out_cache, out_cache
						, out_desired, weights, bias
						, cachedelta, ae2f_AnnActFFN_PASS, ae2f_AnnActDerivFFN_PASS, lossderiv
						, learningrate, learningrate_bias
						);

			}
		}
	}
}


#endif
