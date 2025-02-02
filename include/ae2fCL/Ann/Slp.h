/// @file Slp.h
#ifndef ae2fCL_AnnSlp_h
#define ae2fCL_AnnSlp_h

#include "Sp.h"

typedef struct ae2fCL_AnnSlpEl ae2fCL_AnnSlpEl;
typedef struct ae2fCL_AnnSlp ae2fCL_AnnSlp;

/// @brief
/// An element type of @ref ae2fCL_AnnSlp. \n
/// @ref ae2fCL_AnnSlp has its responsibility for memory.
struct ae2fCL_AnnSlpEl {
    /// @brief 
    /// Local padding index for input memory object.
    size_t InputIdxPad;

    /// @see @ref ae2fCL_AnnSp
    ae2fCL_AnnSp Perceptron[1];

    #if ae2f_WhenCXX(1) + 0
    #include "Slp/SlpEl.hpp"
    #endif
};

/// @brief
/// @param _this
#define ae2fCL_AnnSlpElInLengthReq(_this) (_this->InputIdxPad + _this->Perceptron->mgWeightLen)

/// @brief 
/// # Single Layered Perceptron
///
/// Expecting the implementation of single perceptron,
/// this structure is able to predict multiple output by stacking the perceptron.
struct ae2fCL_AnnSlp {
    /// @brief 
    /// The layer.
    ae2fCL_AnnSlpEl* List;
    
    size_t
    /// @brief
    /// The expected count of prediction, which also means the length of the @ref ae2fCL_AnnSlp::List.
    OutCount,

    /// @brief
    /// The expected count of input as @ref ae2f_float_t. 
    MaxInCount;
};

/// @warning
/// Notice that the macro will not perform check for index and null pointer.
/// @memberof ae2fCL_AnnSlp
/// @param[in] _this [ae2fCL_AnnSlp*]
/// @param i [size_t] index for List. Must not be greater than @ref ae2fCL_AnnSlp::OutCount.
/// @return [ae2fCL_fpAnnAct_t&]
/// Returns the activation function from [i]th element of @ref ae2fCL_AnnSlp::List.
#define ae2fCL_AnnSlpGetAct(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mAct)

/// @warning
/// Notice that the macro will not perform check for index and null pointer.
/// @memberof ae2fCL_AnnSlp
/// @param[in] _this [ae2fCL_AnnSlp*]
/// @param i [size_t] index for List. Must not be greater than @ref ae2fCL_AnnSlp::OutCount.
/// @return [ae2fCL_fpAnnSpGetLoss_t&]
/// Returns the loss function from [i]th element of @ref ae2fCL_AnnSlp::List
#define ae2fCL_AnnSlpGetLoss(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mpGetLoss)

/// @warning
/// It will not check whether the structure is initialised. \n
/// Notice that initialised class must pass the function @ref ae2fCL_AnnSlpDel.
/// @memberof ae2fCL_AnnSlp
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
    ae2fCL_AnnSlp* _this,
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
    ae2fCL_AnnSlp* _this
) noexcept;

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// # Predict the multiple value from multiple input.
/// 
/// @param _this 
/// 
/// @param in 
/// The input memory object. \n
/// Must be longer or same as @code in_idx + _this->MaxInCount
/// 
/// @param out_optionalA 
/// The output memory object (optional)
/// 
/// @param in_idx 
/// The padding index for [in]. \n
/// 
/// @param out_idx_optionalA 
/// 
/// 
/// @param outbuff_optional_ 
/// @param queue 
/// @param blocking_event 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @param context_optionalB 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpPredict(
    const ae2fCL_AnnSlp* _this,
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
/// @param _this 
/// @param in 
/// @param out_optionalA 
/// @param in_idx 
/// @param out_idx_optionalA 
/// @param goal 
/// @param LearningRateGlobal_optional_A 
/// @param LearningRateArr_optional_B 
/// @param diff_ret_optional 
/// @param delta_precalculated 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @param context_optionalB 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpTrain(
    ae2fCL_AnnSlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    const ae2f_float_t* goal,
    ae2f_float_t LearningRateGlobal_optional_A,
    ae2f_float_t* LearningRateArr_optional_B,
    ae2f_float_t* diff_ret_optional,
    const ae2f_float_t* delta_precalculated,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
) noexcept;

#if ae2f_WhenCXX(1) + 0
#include "Slp/imp.hpp"
#endif

#endif