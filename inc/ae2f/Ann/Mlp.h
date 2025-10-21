#ifndef	ae2f_NEED_CLASS
#define ae2f_NEED_CLASS 1
#endif

#ifndef ae2f_Ann_Mlp_h
#define ae2f_Ann_Mlp_h

/** TODO: Separate header and macros */
#include "./Mlp.core.h"

#if ae2f_NEED_CLASS
#include <ae2f/Ann/Act.h>
#endif

#include "./Slp.h"
#include <ae2f/Pack/Beg.h>
#include <assert.h>

/**
 * @brief
 * # Multi Layered Perceptron
 *
 * This is the main structure for the MLP.
 */
ae2f_structdef(struct, ae2f_AnnMlp)
#if	ae2f_NEED_CLASS
{
	/**
	 * @brief
	 * Depth of the network, including input and output layers.
	 */
	size_t			m_depth;

	/**
	 * @brief
	 * Possible greatest output size.
	 */
	size_t			m_outc;

	/** Possible greatest weight page size */
	size_t			m_weightc;

	/**
	 * @brief
	 * Vector of layer sizes.
	 */
	ae2f_LP(m_depth) const size_t* ae2f_restrict	m_sz;

	/**
	 * @brief
	 * Weights of the network.
	 */
	ae2f_LP(m_outc * m_outc * (m_depth - 1)) ae2f_float_t* ae2f_restrict	m_weight;
	/**
	 * @brief
	 * Biases of the network.
	 */
	ae2f_LP(m_outc * (m_depth - 1)) ae2f_float_t* ae2f_restrict		m_bias;
	/**
	 * @brief
	 * Stream for delta values during backpropagation.
	 */
	ae2f_LP(m_outc * (m_depth - 1)) ae2f_float_t* ae2f_restrict		m_deltastream;
	/**
	 * @brief
	 * Cache for layer outputs.
	 */
	ae2f_LP(m_outc * (m_depth - 1)) ae2f_float_t* ae2f_restrict		m_outcache;

	/**
	 * @brief
	 * Activation functions for each layer.
	 */
	ae2f_LP(m_depth - 1) ae2f_AnnActFFN_t** ae2f_restrict	m_act;
	/**
	 * @brief
	 * Derivatives of activation functions for each layer.
	 */
	ae2f_LP(m_depth - 1) ae2f_AnnActFFN_t** ae2f_restrict	m_actderiv;
	/**
	 * @brief
	 * Derivative of the loss function.
	 */
	ae2f_AnnLossFFN_t*					m_lossderiv;

	/**
	 * @brief
	 * Learning rate for weights.
	 */
	ae2f_float_t					m_learningrate;
	/**
	 * @brief
	 * Learning rate for biases.
	 */
	ae2f_float_t					m_learningrate_bias;

#if ae2f_WhenCXX(!)0
#undef	ae2f_TMP
#define	ae2f_TMP

	inline void ae2f_TMP Predict(
			ae2f_opt ae2f_err_t* ae2f_restrict		reterr
			, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
			, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
			) const ae2f_noexcept;

	inline void ae2f_TMP PredictStream(
			ae2f_opt ae2f_err_t* ae2f_restrict		reterr
			, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
			, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
			) const ae2f_noexcept;

	inline void ae2f_TMP Follow(
			ae2f_opt ae2f_err_t* const	reterr
			, const ae2f_float_t* const	inp	ae2f_LP(mlp::m_sz[0])
			, const ae2f_float_t* const	delta	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

	inline void ae2f_TMP FollowStream(
			ae2f_opt ae2f_err_t* const	reterr
			, const ae2f_float_t* const	inp	ae2f_LP(mlp::m_sz[0])
			, const ae2f_float_t* const	delta	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

	inline void ae2f_TMP Train(
			ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
			, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
			, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
			, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

	inline void ae2f_TMP TrainStream(
			ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
			, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
			, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
			, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

	inline void ae2f_TMP TrainAuto(
			ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
			, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
			, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

	inline void ae2f_TMP TrainAutoStream(
			ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
			, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
			, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
			) ae2f_noexcept;

#endif
}
#endif
;

#include <ae2f/Pack/End.h>


#if ae2f_MAC_BUILD && 0

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpMk(
		ae2f_opt ae2f_err_t* ae2f_restrict const	reterr
		, ae2f_AnnMlp* ae2f_restrict* ae2f_restrict const	retmk				ae2f_FREE(ae2f_AnnMlpDel)

		, const size_t				depth
		, const size_t* ae2f_restrict const		szvector				ae2f_LP(depth)
		, ae2f_opt size_t* ae2f_restrict const	szswap_opt				ae2f_LP(depth)

		, ae2f_opt ae2f_AnnActFFN_t** ae2f_restrict const	act				ae2f_LP(depth - 1)
		, ae2f_opt ae2f_AnnActFFN_t** ae2f_restrict const	actderiv			ae2f_LP(depth - 1)
		, ae2f_AnnLoss_t* const				lossderiv

		, ae2f_opt ae2f_float_t* ae2f_restrict const	deltastream	ae2f_LP(max(szvector) * (depth - 1))
		, ae2f_opt ae2f_float_t* ae2f_restrict const	outcache	ae2f_LP(max(szvector) * (depth - 1))
		, ae2f_opt ae2f_float_t* ae2f_restrict const	weight	ae2f_LP(pow(max(szvector), 2) * (depth - 1))
		, ae2f_opt ae2f_float_t* ae2f_restrict const	bias				ae2f_LP(max(szvector) * (depth - 1))

		, ae2f_float_t const	learningrate
		, ae2f_float_t const	learningrate_bias

		, ae2f_opt const size_t	offset
, ae2f_opt const size_t	extra
) ae2f_noexcept;

ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpDel(
		ae2f_AnnMlp* ae2f_restrict const block
		) ae2f_noexcept;

/**
 * @brief
 * Predict the output from mlp.
 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpPredict(
		ae2f_opt ae2f_err_t* ae2f_restrict		reterr
		, const ae2f_AnnMlp* ae2f_restrict const	mlp
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/**
	 * @brief
	 * Predict the output from mlp.
	 *
	 * @details
	 * Every output calculated for each layer will be stored on mlp->m_outcache
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpPredictStream(
		ae2f_err_t* ae2f_restrict			reterr
		, const ae2f_AnnMlp* ae2f_restrict const	mlp
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given delta for last layer. 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpFollow(
		ae2f_opt ae2f_err_t* ae2f_restrict const	reterr
		, const ae2f_AnnMlp* ae2f_restrict		mlp
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	delta	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given delta for last layer. 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpFollowStream(
		ae2f_opt ae2f_err_t* ae2f_restrict const	reterr
		, const ae2f_AnnMlp* ae2f_restrict		mlp
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	delta	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given input and output_desired 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpTrain(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* ae2f_restrict const		mlp
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given input and output_desired 
	 * Previous output will be 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpTrainStream(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* ae2f_restrict const		mlp
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given input and output_desired 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpTrainAuto(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* ae2f_restrict const		mlp
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

	/** 
	 * @brief 
	 * Adjusts the weights and biases with given input and output_desired 
	 * Previous output will be 
	 * */
ae2f_extern ae2f_SHAREDCALL void ae2f_AnnMlpTrainAutoStream(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, ae2f_AnnMlp* ae2f_restrict const		mlp
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept;

#else

#define ae2f_AnnMlpMk	__ae2f_AnnMlpMk_C
#define ae2f_AnnMlpDel	__ae2f_AnnMlpDel_C

#define ae2f_AnnMlpPredict		__ae2f_AnnMlpPredict_C
#define ae2f_AnnMlpPredictStream	__ae2f_AnnMlpPredictStream_C
#define ae2f_AnnMlpFollow		__ae2f_AnnMlpFollow_C
#define ae2f_AnnMlpFollowStream		__ae2f_AnnMlpFollowStream_C
#define ae2f_AnnMlpTrain		__ae2f_AnnMlpTrain_C
#define ae2f_AnnMlpTrainStream		__ae2f_AnnMlpTrainStream_C
#define ae2f_AnnMlpTrainAuto		__ae2f_AnnMlpTrainAuto_C
#define ae2f_AnnMlpTrainAutoStream	__ae2f_AnnMlpTrainAutoStream_C


#endif

#include "./Mlp.auto.h"

#if ae2f_WhenCXX(!)0 && ae2f_NEED_CLASS

	/** 
	 * @FIXME 
	 * It somehow compiles and I see redefining errors. 
	 * */
#undef	ae2f_TMP
#define	ae2f_TMP ae2f_AnnMlp::
inline void ae2f_TMP Predict(
		ae2f_err_t* ae2f_restrict			reterr
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
		) const ae2f_noexcept {
	ae2f_AnnMlpPredict(reterr, this, inp, out);
}

inline void ae2f_TMP PredictStream(
		ae2f_err_t* ae2f_restrict			reterr
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out	ae2f_LP(mlp::m_sz[fin])
		) const ae2f_noexcept {
	ae2f_AnnMlpPredictStream(reterr, this, inp, out);
}

inline void ae2f_TMP Follow(
		ae2f_err_t* const		reterr
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	delta	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpFollow(reterr, this, inp, delta);
}

inline void ae2f_TMP FollowStream(
		ae2f_err_t* const		reterr
		, const ae2f_float_t* ae2f_restrict const	inp	ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	delta	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpFollowStream(reterr, this, inp, delta);
}

inline void ae2f_TMP Train(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpTrain(reterr, this, inp, out, out_desired);
}

inline void ae2f_TMP TrainStream(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, const ae2f_float_t* ae2f_restrict const	inp				ae2f_LP(mlp::m_sz[0])
		, ae2f_float_t* ae2f_restrict const		out				ae2f_LP(mlp::m_sz[fin])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpTrainStream(reterr, this, inp, out, out_desired);
}

inline void ae2f_TMP TrainAuto(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, const ae2f_float_t* ae2f_restrict const	inp		ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpTrainAuto(reterr, this, inp, out_desired);
}

inline void ae2f_TMP TrainAutoStream(
		ae2f_err_t* ae2f_restrict const ae2f_opt	reterr
		, const ae2f_float_t* ae2f_restrict const	inp		ae2f_LP(mlp::m_sz[0])
		, const ae2f_float_t* ae2f_restrict const	out_desired	ae2f_LP(mlp::m_sz[fin])
		) ae2f_noexcept {
	ae2f_AnnMlpTrainAutoStream(reterr, this, inp, out_desired);
}


#endif

#define	__ae2f_AnnMlpTrain_C(reterr, mlp, inp, out, out_desired) \
	__ae2f_AnnMlpTrainPrimal(&1 ? 0 : 1, &1, reterr, mlp, inp, out, out_desired)

#define	__ae2f_AnnMlpTrainStream_C(reterr, mlp, inp, out, out_desired) \
	__ae2f_AnnMlpTrainPrimal(&1 ? 0 : 1, ae2f_NONE, reterr, mlp, inp, out, out_desired)

#define	__ae2f_AnnMlpTrain_imp(v_train, mlp, inp, out, goal, lenv, outstream, deltacache, weight, bias, lr_w, lr_b, act, actderiv, lossderiv) \
	__ae2f_AnnMlpTrainPrimal_imp(&1 ? 0 : 1, &1, v_train, mlp, inp, out, goal, lenv, outstream, deltacache, weight, bias, lr_w, lr_b, act, actderiv, lossderiv) 

#define	__ae2f_AnnMlpTrainStream_imp(v_train, mlp, inp, out, goal, lenv, outstream, deltacache, weight, bias, lr_w, lr_b, act, actderiv, lossderiv) \
	__ae2f_AnnMlpTrainPrimal_imp(-1, ae2f_NONE, v_train, mlp, inp, out, goal, lenv, outstream, deltacache, weight, bias, lr_w, lr_b, act, actderiv, lossderiv)


/** @see __ae2f_AnnMlpTrainAutoPrimal */
#define	__ae2f_AnnMlpTrainAuto_C(reterr, mlp, inp, out_desired) \
	__ae2f_AnnMlpTrainAutoPrimal(&1 ? 0 : 1, &1, reterr, mlp, inp, out_desired)

/** @see __ae2f_AnnMlpTrainAutoPrimal */
#define	__ae2f_AnnMlpTrainAutoStream_C(reterr, mlp, inp, out_desired) \
	__ae2f_AnnMlpTrainAutoPrimal(-1, ae2f_NONE, reterr, mlp, inp, out_desired)

#endif
