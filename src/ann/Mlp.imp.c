#include "ae2f/Call.h"
#include "ae2f/Cast.h"
#include <ae2f/Ann/Mlp.h>

ae2f_SHAREDEXPORT void ae2f_AnnMlpMk(
		ae2f_err_t* restrict const		reterr
		, ae2f_AnnMlp* restrict* restrict const	retmk

		, const size_t				depth
		, const size_t* restrict const		szvector
		, ae2f_opt size_t* restrict const	szswap_opt

		, ae2f_opt ae2f_AnnAct_t** restrict const	act
		, ae2f_opt ae2f_AnnAct_t** restrict const	actderiv
		, ae2f_AnnLoss_t* const			lossderiv

		, ae2f_opt ae2f_float_t* restrict const	deltastream
		, ae2f_opt ae2f_float_t* restrict const	outcache
		, ae2f_opt ae2f_float_t* restrict const	weight
		, ae2f_opt ae2f_float_t* restrict const	bias

		, ae2f_float_t const	learningrate
		, ae2f_float_t const	learningrate_bias
		) noexcept
{
	__ae2f_AnnMlpMk_C(
			reterr
			, retmk, depth
			, szvector, szswap_opt
			, act, actderiv, lossderiv
			, deltastream, outcache, weight
			, bias, learningrate, learningrate_bias
			);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpDel(
		ae2f_AnnMlp* restrict const block
		) noexcept {
	__ae2f_AnnMlpDel_C(block);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpPredict(
		ae2f_err_t* restrict			reterr
		, const ae2f_AnnMlp* restrict const	mlp
		, const ae2f_float_t* restrict const	inp
		, ae2f_float_t* restrict const		out
		) noexcept
{
	__ae2f_AnnMlpPredict_C(reterr, mlp, inp, out);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpPredictStream(
		ae2f_err_t* restrict			reterr
		, const ae2f_AnnMlp* restrict const	mlp
		, const ae2f_float_t* restrict const	inp
		, ae2f_float_t* restrict const		out
		) noexcept
{
	__ae2f_AnnMlpPredictStream_C(reterr, mlp, inp, out);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpFollow(
		ae2f_err_t* const		reterr
		, const ae2f_AnnMlp*		mlp
		, const ae2f_float_t* const	inp
		, const ae2f_float_t* const	delta
		)
{
	__ae2f_AnnMlpFollow_C(reterr, mlp, inp, delta);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpFollowStream(
		ae2f_err_t* const		reterr
		, const ae2f_AnnMlp*		mlp
		, const ae2f_float_t* const	inp
		, const ae2f_float_t* const	delta
		)
{
	__ae2f_AnnMlpFollowStream_C(reterr, mlp, inp, delta);
}


ae2f_SHAREDEXPORT void ae2f_AnnMlpTrain(
		ae2f_err_t* restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* restrict const		mlp
		, const ae2f_float_t* restrict const	inp
		, ae2f_float_t* restrict const		out
		, const ae2f_float_t* restrict const	out_desired
		) noexcept {
	__ae2f_AnnMlpTrain_C(reterr, mlp, inp, out, out_desired);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpTrainStream(
		ae2f_err_t* restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* restrict const		mlp
		, const ae2f_float_t* restrict const	inp
		, ae2f_float_t* restrict const		out
		, const ae2f_float_t* restrict const	out_desired
		) noexcept {
	__ae2f_AnnMlpTrainStream_C(reterr, mlp, inp, out, out_desired);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpTrainAuto(
		ae2f_err_t* restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* restrict const		mlp
		, const ae2f_float_t* restrict const	inp
		, const ae2f_float_t* restrict const	out_desired
		) noexcept {
	__ae2f_AnnMlpTrainAuto_C(reterr, mlp, inp, out_desired);
}

ae2f_SHAREDEXPORT void ae2f_AnnMlpTrainAutoStream(
		ae2f_err_t* restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* restrict const		mlp
		, const ae2f_float_t* restrict const	inp
		, const ae2f_float_t* restrict const	out_desired
		) noexcept {
	__ae2f_AnnMlpTrainAutoStream_C(reterr, mlp, inp, out_desired);
}
