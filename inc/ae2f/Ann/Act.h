/**
 * @file vAct.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2fCL_AnnAct_h
#define ae2fCL_AnnAct_h

#include <stddef.h>
#include <ae2f/Float.h>
#include <ae2f/Guide.h>

/**
 * @typedef ae2f_AnnActFFN_t
 * @brief Customisable activasion function type for FFN(Mlp). 
 * */
typedef void ae2f_AnnActFFN_t (
		ae2f_float_t*				ret
		, ae2f_LP(count) const ae2f_float_t*	out
		, const size_t				index
		, const size_t				count
		);

#define ae2f_AnnActFFN_PASS(r, o, i, c)		*(r) = (o)[i]
#define ae2f_AnnActDerivFFN_PASS(r, o, i, c)	*(r) = 1.0

/**
 @brief
 Specify the way of calculating loss for FFN(Mlp).
 
 This will be multiplied with learning rate, \n
 and that value will be added to each weights field and bias.
 @param out Predicted value
 @param goal Expected value (wanted)
 @param ret Calculated loss.
 */
typedef void ae2f_AnnLossFFN_t (
		ae2f_float_t* 				ret,
		ae2f_LP(count) const ae2f_float_t*	out, 
		ae2f_LP(count) const ae2f_float_t* 	goal,
		const size_t				index,
		const size_t				count
		);

#define ae2f_AnnLossDerivFFN_PASS(r, o, g, i, c)	*(r) = (o)[i] * (1.0 - (g)[i]);

typedef void ae2f_AnnActFwdMHATTN_t(
		ae2f_float_t* 				ret,
		size_t					prm_retidx,
		ae2f_LP(prm_len) const ae2f_float_t*	prm_inp,
		size_t					prm_len
		);

typedef void ae2f_AnnActBwdMHATTN_t(
		ae2f_float_t*				ret,
		size_t					prm_retidx,
		ae2f_LP(prm_len) const ae2f_float_t*	prm_grad_in,
		ae2f_LP(prm_len) const ae2f_float_t*	prm_softmax_out,
		size_t					prm_len
		);

#endif
