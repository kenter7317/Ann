#include <ae2f/MMap/Util.h>

#if !ae2f_MAC_BUILD
#error "wtf bro"
#endif

#include <ae2f/Ann/Slp.h>
#include <ae2f/Cast.h>
#include <stdlib.h>

ae2f_SHAREDEXPORT void ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* ae2f_restrict const _this,
		const size_t inc,
		const size_t outc,
		const size_t offset_opt,
		ae2f_err_t* ae2f_restrict  const err_opt,
		size_t* ae2f_restrict const initsz_opt
		) ae2f_noexcept
__ae2f_AnnSlpInit_C(
		_this
		, inc, outc
		, offset_opt, err_opt, initsz_opt
		);


ae2f_SHAREDEXPORT void ae2f_AnnSlpMk(
		ae2f_LP(inc * outc) ae2f_float_t* ae2f_restrict const	weight_opt,
		ae2f_LP(outc) ae2f_float_t* ae2f_restrict const 		bias_opt,
		ae2f_LP(outc) ae2f_float_t* ae2f_restrict const 		cache_opt,

		const size_t					inc,
		const size_t					outc,
		const size_t					offset_opt,
		const size_t					extra_opt,
		ae2f_FREE(ae2f_AnnSlpDel, __ae2f_AnnSlpDel) 
		ae2f_AnnSlp* ae2f_restrict * ae2f_restrict const 				slp,
		ae2f_opt ae2f_AnnAct_t*  const			act,
		ae2f_opt ae2f_AnnAct_t* const			actderiv,
		ae2f_AnnLoss_t* const				lossderiv,
		ae2f_float_t					learningrate,
		ae2f_float_t					learningrate_bias,
		ae2f_opt ae2f_err_t* ae2f_restrict const			err_opt
		) ae2f_noexcept {
	__ae2f_AnnSlpMk_C(
			weight_opt, bias_opt, cache_opt
			, inc, outc
			, offset_opt, extra_opt, slp
			, act, actderiv, lossderiv
			, learningrate, learningrate_bias
			, err_opt)
}



ae2f_SHAREDEXPORT void ae2f_AnnSlpDel(ae2f_AnnSlp* ae2f_restrict const slp) 
	ae2f_noexcept
{ __ae2f_AnnSlpDel(slp); }

ae2f_SHAREDEXPORT void ae2f_AnnSlpPredict(
		ae2f_err_t* ae2f_restrict const err_opt
		, const ae2f_AnnSlp* ae2f_restrict const _this
		, const ae2f_float_t* ae2f_restrict const prm_in
		, ae2f_float_t* ae2f_restrict const out
		) ae2f_noexcept
{
	__ae2f_AnnSlpPredict_C(
			err_opt, _this
			, prm_in, out
			);
}


ae2f_SHAREDEXPORT void ae2f_AnnSlpFollow(
		ae2f_err_t* const ae2f_restrict reterr_opt
		, const ae2f_AnnSlp* ae2f_restrict const _this
		, const ae2f_float_t* ae2f_restrict const prm_in
		, const ae2f_float_t* ae2f_restrict const delta
		) ae2f_noexcept {
	__ae2f_AnnSlpFollow_C(reterr_opt, _this, prm_in, delta);
}

ae2f_SHAREDEXPORT void ae2f_AnnSlpFit(
		ae2f_err_t* ae2f_restrict const reterr_opt
		, const ae2f_AnnSlp* ae2f_restrict const _this
		, ae2f_LP(_this::inc) const ae2f_float_t* ae2f_restrict const prm_inp
		, ae2f_LP(_this::outc) const ae2f_float_t* ae2f_restrict const prm_out
		, ae2f_LP(_this::outc) const ae2f_float_t* ae2f_restrict const prm_out_desired
		) ae2f_noexcept
{
	__ae2f_AnnSlpFit_C(
			reterr_opt, _this
			, prm_inp
			, prm_out, prm_out_desired
			);
}

ae2f_SHAREDEXPORT void ae2f_AnnSlpFetchDelta(
		ae2f_opt ae2f_err_t* ae2f_restrict const			err
		, const ae2f_AnnSlp* ae2f_restrict				slp

		, ae2f_LP(slp::outc) const ae2f_float_t* ae2f_restrict const	out
		, ae2f_LP(slp::outc) const ae2f_float_t* ae2f_restrict const	out_desired

		, ae2f_LP(slp::outc) ae2f_float_t* ae2f_restrict const	retdelta
		) ae2f_noexcept
{
	__ae2f_AnnSlpFetchDelta_C(err, slp, out, out_desired, retdelta);
}

ae2f_SHAREDEXPORT void ae2f_AnnSlpTrain(
		ae2f_err_t* ae2f_restrict const				err
		, ae2f_AnnSlp* ae2f_restrict					slp
		, ae2f_LP(slp::inc)	const ae2f_float_t* ae2f_restrict	inp
		, ae2f_LP(slp::outc)	const ae2f_float_t* ae2f_restrict	out_desired
		) ae2f_noexcept
{
	__ae2f_AnnSlpTrain_C(err, slp, inp, out_desired);
}

