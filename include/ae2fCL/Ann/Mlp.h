#ifndef ae2fCL_Ann_Mlp_h
#define ae2fCL_Ann_Mlp_h

#include <ae2f/Ann/Mlp.h>
#include <ae2fCL/Ann.h>

/**
 * @brief
 * Initialise, and make additional allocations for the model. \n
 * It could be cleaned with @ref ae2f_AnnMlpClean
 *
 * @param layerc
 * The length for almost any kind of vectors.
 *
 * @param act_deriv_v_opt
 * Its length is layerc - 1
 *
 * It will be for each layer of perceptron's 
 * delta-to-delta calculation.
 *
 * @param delta_opt
 * This one function pointer is for delta calculatoin from goal,
 * for last layer, which could get the actual goal(expected output) 
 * as it seem.
 *
 * */
ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_AnnMlpInit(
	ae2f_mAnnMlp* _this,
	size_t layerc,
	size_t add_opt,
	const size_t* layerlenv,
	const size_t* layerpadv_opt_unused,
	const size_t* inpadv_opt,
	const ae2f_fpAnnAct_t* actv_opt,
	const ae2f_fpAnnAct_t* act_deriv_v_opt,
	const ae2f_fpAnnLoss_t* lossderiv_v_opt,
	const ae2f_float_t* weights_opt,
	ae2f_err_t* errret_opt,
	cl_int* errnfound_opt
) noexcept;

/**
 * @brief
 * Makes an actual instance of MLP. \n
 * It could be released with
 * @ref ae2f_AnnMlpDel
 * or simply `delete` in a context of C++.
 *
 * @param layerc
 * The length for almost any kind of vectors.
 *
 * @param act_deriv_v_opt
 * Its length is layerc - 1.
 * 
 * It will be for each layer of perceptron's 
 * delta-to-delta calculation.
 *
 * @param delta_opt
 * This one function pointer is for delta calculatoin from goal,
 * for last layer, which could get the actual goal(expected output) 
 * as it seem.
 * */
ae2f_extern ae2f_SHAREDCALL
ae2f_AnnMlp* ae2fCL_AnnMlpMk(
	size_t layerc,
	size_t add_opt,
	const size_t* layerlenv,
	const size_t* layerpadv_opt,
	const size_t* inpadv_opt,
	const ae2f_fpAnnAct_t* actv_opt,
	const ae2f_fpAnnAct_t* act_deriv_v_opt,
	const ae2f_fpAnnLoss_t* lossderiv_v_opt,
	const ae2f_float_t* weights_opt,
	ae2f_err_t* errret_opt,
	cl_int* errnfound_opt
) noexcept;

#define ae2fCL_mAnnMlpInitSz ae2f_mAnnMlpInitSz

#endif
