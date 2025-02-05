/**
 * @file Slp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2fCL_AnnSlp_h
#define ae2fCL_AnnSlp_h

#include "Sp.h"

/// @brief
/// An element type of @ref ae2fCL_AnnSlp. \n
/// @ref ae2fCL_AnnSlp has its responsibility for memory.
typedef struct ae2fCL_AnnSlpEl {
    /// @brief 
    /// Local padding index for input memory object.
    size_t InputIdxPad;

    /// @see @ref ae2fCL_AnnSp
    ae2fCL_AnnSp Perceptron[1];

    #if ae2f_WhenCXX(1) + 0
    #include "Slp.h.cxx/SlpEl.hpp"
    #endif
} ae2fCL_AnnSlpEl;

/// @memberof ae2fCL_AnnSlpEl
/// @brief
/// @param _this
#define ae2fCL_AnnSlpElInLengthReq(_this) (_this->InputIdxPad + _this->Perceptron->mgWeightLen)

/// @brief 
/// # Single Layered Perceptron
/// 
/// Expecting the implementation of single perceptron,
/// this structure is able to predict multiple output by stacking the perceptron.
typedef struct ae2fCL_AnnSlp {
    /// @brief 
    /// The layer (aka list).
    ae2fCL_AnnSlpEl* List;
    
    size_t
    /// @brief
    /// The expected count of prediction, which also means the length of the @ref ae2fCL_AnnSlp::List.
    OutCount,

    /// @brief
    /// The expected count of input as @ref ae2f_float_t. 
    MaxInCount;

    #if ae2f_WhenCXX(1) + 0
    #include "Slp.h.cxx/Slp.hpp"
    #endif
} ae2fCL_AnnSlp;


/// @memberof ae2fCL_AnnSlp
/// @warning
/// Notice that the macro will not perform check for index and null pointer.
/// @param[in] _this [ae2fCL_AnnSlp*]
/// @param i [size_t] index for List. Must not be greater than @ref ae2fCL_AnnSlp::OutCount.
/// @return [ae2fCL_fpAnnAct_t&]
/// Returns the activation function from [i]th element of @ref ae2fCL_AnnSlp::List.
#define ae2fCL_AnnSlpGetAct(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mAct)

/// @memberof ae2fCL_AnnSlp
/// @warning
/// Notice that the macro will not perform check for index and null pointer.
/// @param[in] _this [ae2fCL_AnnSlp*]
/// @param i [size_t] index for List. Must not be greater than @ref ae2fCL_AnnSlp::OutCount.
/// @return [ae2fCL_fpAnnSpGetLoss_t&]
/// Returns the loss function from [i]th element of @ref ae2fCL_AnnSlp::List
#define ae2fCL_AnnSlpGetLoss(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mpGetLoss)

/// @memberof ae2fCL_AnnSlp
/// @warning
/// It will not check whether the structure is initialised. \n
/// Notice that initialised class must pass the function @ref ae2fCL_AnnSlpDel.
/// @brief 
/// Initialise the class.
/// 
/// @param[out] _this 
/// @param[in] inputsCount_optionalA
/// A list that contains each perceptron's suppossed input length. \n
/// Its readable length must be same or more as [outputCount].
/// 
/// @param[in] padCount_optional
/// A list that contains each perceptron's supposed input pad index. \n
/// Its readable length must be same or more as [outputCount].
/// 
/// @param[in] inputsCountGlobal
/// Suggested input size for this model.
/// 
/// @param outputCount 
/// Suggested output size for this model.
/// 
/// @param mAct
/// Activation function.
///  
/// @param fpGetLoss 
/// Delta calculation function.
///
/// @param ctx 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMk(
    ae2f_struct ae2fCL_AnnSlp* _this,
    const size_t* inputsCount_optionalA,
    const size_t* padCount_optional,
    size_t inputsCountGlobal,
    size_t outputCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSpGetLoss_t fpGetLoss,
    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) noexcept;

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// Release the allocated resource of [_this].
/// @param[in, out] _this 
/// Its resources will be freed.
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpDel(
    ae2f_struct ae2fCL_AnnSlp* _this
) noexcept;

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// # Predicts the multiple value from multiple input.
/// 
/// @param[in] _this 
/// 
/// @param[in] in 
/// The input memory object. \n
/// The buffer's length will be considered as @code in_idx + _this->MaxInCount
/// 
/// @param out_optionalA 
/// The output memory object (optional) \n
/// Setting it null will let this function dynamically allocate a temporaral buffer. 
/// 
/// The buffer's length will be considered as @code out_idx_optionalA + _this->OutCount
/// 
/// @param in_idx 
/// The padding index for [in]. \n
/// 
/// @param out_idx_optionalA 
/// The padding index for [out_optionalA].
/// 
/// @param[out] outbuff_optional_ 
/// A pointer where output will be passed. \n
/// Its length is considered as [OutCount]
/// 
/// @param queue 
/// @param blocking_event 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @param context_optionalB 
/// OpenCL Context in a case that OpenCL Memory Object allocation is needed.
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpPredict(
    const ae2f_struct ae2fCL_AnnSlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,
    ae2f_float_t* outbuff_optional_,
    cl_command_queue queue,
    cl_bool blocking_event,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context_optionalB
) noexcept;

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// Fits this layer for given goal. \n
/// It will roughly think for given input [in] the output have to be [goal].
/// 
/// @param[in, out] _this 
/// @param[in] in 
/// Given Input for this class. \n
/// It's length is considered as @code in_idx + _this->MaxInCount
/// 
/// @param[out] out_optionalA
/// 
/// Output where its output will be located. \n
/// This is needed when this class is trying to calculate a Loss from Prediction.
///
/// Setting it null would cause OpenCL Memory object allocation and freeing. \n
/// See [context_optionalB].
///
/// This behaviour will not be necessary given pre-calculated loss(delta).
/// See [delta_precalculated_optC].
///  
/// @param in_idx 
/// Padding index for [in].
/// @param out_idx_optionalA
/// Padding index for [out]. 
/// @param[in] goal 
/// Wanted output for [in]. \n
/// It is no necessary when [delta_precalculated_optC] is provided.
/// 
/// @param LearningRateGlobal_optional_A
/// When LearningRateArr_optional_B is null, 
/// you could pass this argument as 
/// 
/// @param[in] LearningRateArr_optional_B
/// In a case you want to specify every learning rate for each perceptron. \n
/// Its length is expected as a count of @code _this->OutCount.
/// 
/// @param[out] diff_ret_optional
/// Calculated delta will be stored here
/// 
/// @param[in] delta_precalculated_optC
/// In a case you want to specify deltas for each perceptron, you could pass here. \n
/// Its length is expected as a count of @code _this->OutCount.
/// 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param event 
/// @param context_optionalB 
/// OpenCL Context in a case that OpenCL Memory Object allocation is needed.
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpTrain(
    ae2f_struct ae2fCL_AnnSlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    const ae2f_float_t* goal,
    ae2f_float_t LearningRateGlobal_optional_A,
    ae2f_float_t* LearningRateArr_optional_B,
    ae2f_float_t* diff_ret_optional,
    const ae2f_float_t* delta_precalculated_optC,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
) noexcept;

#if ae2f_WhenCXX(1) + 0
#include "Slp.h.cxx/imp.hpp"
#endif

#endif