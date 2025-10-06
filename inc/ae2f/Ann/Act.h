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

/// @brief Customisable activasion function type.
typedef void ae2f_AnnAct_t (
		ae2f_float_t*				ret
		, ae2f_LP(count) const ae2f_float_t*	out
		, const size_t				index
		, const size_t				count
		);

#define ae2f_AnnAct_PASS(r, o, i, c)		*(r) = (o)[i]
#define ae2f_AnnActDeriv_PASS(r, o, i, c)	

/// @brief
/// Specify the way of calculating loss.
/// 
/// This will be multiplied with learning rate, \n
/// and that value will be added to each weights field and bias.
/// @param out Predicted value
/// @param goal Expected value (wanted)
/// @return Calculated loss.
typedef void ae2f_AnnLoss_t (
		ae2f_float_t* 				ret,
		ae2f_LP(count) const ae2f_float_t*	out, 
		ae2f_LP(count) const ae2f_float_t* 	goal,
		const size_t				index,
		const size_t				count
		);

#define ae2f_AnnLossDeriv_PASS(r, o, g, i, c)	*(r) = (o)[i] * (1.0 - (g)[i]);

#endif
