/**
 * @file Sp.h
 * @author ae2f
 * @brief 
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ae2fCL_Ann_Slp_h
#define ae2fCL_Ann_Slp_h

#include <ae2f/Cast.h>
#include <ae2f/Call.h>
#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2f/errGlob.h>

#include "Act.h"
#include <ae2f/Pack/Beg.h>

/// @brief
/// Specify the way of calculating loss.
/// 
/// This will be multiplied with learning rate, \n
/// and that value will be added to each weights field and bias.
/// @param out Predicted value
/// @param goal Expected value (wanted)
/// @return Calculated loss.
typedef ae2f_float_t(*ae2fCL_fpAnnSpGetLoss_t)
    ae2fCL_whenC((ae2f_float_t out, ae2f_float_t goal));

typedef ae2fCL_HostPtr(__global, void) ae2fCL_memobj_void_t;
typedef ae2fCL_HostPtr(__global, ae2f_float_t) ae2fCL_memobj_float_t;

struct ae2fCL_AnnSp;

#include <ae2f/Pack/End.h>

#ifndef ae2fCL_LocAsCL

/// @memberof ae2fCL_AnnSp
/// @brief 
/// Initialises the class. \n
/// The perceptron made by this must pass [ae2fCL_AnnSpDel] for releasing the resources.
/// @param[out] _this 
/// @param[in] weights_optional 
/// Array of weights in a count of [inputsCount] for [ae2fCL_AnnSp::mgWeight]. \n
/// Passing this argument zero will cause [ae2fCL_AnnSp::mgWeight] to be randomised.
/// @param inputsCount
/// Possible weight count. 
/// @param mAct 
/// Activasion function.
/// @param fpGetLoss 
/// Loss calculation.
/// @param ctx 
/// OpenCL context for allocating the memory (like memory object).
/// @param queue 
/// OpenCL Queue for enquing reading-writing.
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event 
ae2f_extern ae2f_SHAREDCALL ae2f_err_t ae2fCL_AnnSpMk(
    ae2f_struct ae2fCL_AnnSp* _this,
    const ae2f_float_t* weights_optional,
    size_t inputsCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSpGetLoss_t fpGetLoss,

    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) noexcept;

/// @memberof ae2fCL_AnnSp
/// @brief 
/// Release the resource for [_this].
/// @param[in, out] _this 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSpDel(
    ae2f_struct ae2fCL_AnnSp* _this
) noexcept;

/// @memberof ae2fCL_AnnSp
/// @warning 
/// Parameter buffer [_out]'s byte length must be over `_this->mgWeightLen * sizeof(ae2f_float_t)`.
/// @brief
/// Copies all weights to [_out].
/// @param[in] _this [const ae2fCL_AnnSp*] The handler for Weights.
/// @param[out] _out [ae2f_float_t*] Buffer for output.
/// @param queue [cl-command_queue]
#define ae2fCL_AnnSpWeightCheck(_this, _out, queue) \
clEnqueueReadBuffer( \
      queue,  \
      (_this)->mgWeight, \
      CL_TRUE, 0, \
      sizeof(ae2f_float_t) * (_this)->mgWeightLen, \
      (_out), \
      0, 0, 0 \
)

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Predict the output.
/// 
/// Each input has a corresponding weight as a Slp.
///
/// This function will calculate the total sum of the multiplication of each input and corresponding weight. \n
/// That value with a bias of the model will pass an activasion function, and that will be considered as an output.
/// 
/// # There are options laballed.
/// 
/// It means when if you see the arguments. \n
/// Some of the paramaters may be ignored since past parameter fulfilled the particular condition.
/// 
/// For specified behaviour, see the macros below:
/// @see ae2fCL_AnnSpPredictA
/// @see ae2fCL_AnnSpPredictB
/// 
/// @param[in] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// @param[out] out_optionalA
/// Writable memory object to use as a buffer for as length as [ae2f_float_t].
/// When set to zero, it will allocate the new one.
/// @param in_idx Padding for [in].
/// @param out_idx_optionalA 
/// Paddings for [out_optionalA]. \n
/// Would not matter a thing when 0 has passed via [out_optionalA].
/// 
/// @param[out] outbuff_optional_ 
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// Its length will be same as [ae2f_float_t].
/// @param queue 
/// @param blocking_event 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event 
/// @param context_optionalB
/// The OpenCL Context in order to allocate the new buffer when needed. 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSpPredict(
    const ae2f_struct ae2fCL_AnnSp* _this,
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

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Predict the output.
/// 
/// See [ae2fCL_AnnSpPredict]
/// 
/// # Option A
/// 
/// This option is taking previously allocated OpenCL Memory Object,
/// therefore it has no need to allocate new buffer to store the first total sum calculated.
/// 
/// @param[in] _this [const ae2fCL_AnnSp*]
/// @param[in] in [cl_mem]
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// @param[out] out [cl_mem]
/// Writable memory object to use as a buffer for as length as [ae2f_float_t].
/// When set to zero, it will the operation trauncated.
/// @param in_idx [uint32_t]
/// Padding for [in].
/// @param out_idx [uint32_t]
/// Paddings for [out]. \n
/// Would not matter a thing when 0 has passed via [out].
/// 
/// @param[out] outbuff_optional [ae2f_float_t*]
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// Its length will be same as [ae2f_float_t].
/// @param queue [cl_command_queue]
/// @param blocking_event  [cl_bool]
/// @param num_events_in_wait_list [cl_uint]
/// @param[in] event_wait_list [const cl_event*]
/// @param[out] event [cl_event*]
#define ae2fCL_AnnSpPredictA(_this, in, out, in_idx, out_idx, outbuff_optional, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSpPredict( \
    _this, in, out, in_idx, out_idx, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, 0\
)

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Predict the output.
/// 
/// See [ae2fCL_AnnSpPredict]
/// 
/// # Option B
/// 
/// This option allocates new OpenCL Memory Object, so you don't need to manually handle.
/// 
/// @param[in] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// @param in_idx Padding for [in].
/// @param[out] outbuff_optional
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// Its length will be same as [ae2f_float_t].
/// @param context
/// The OpenCL Context in order to allocate the new buffer when needed. \n
/// Passing zero here will cause undefined behaviour. 
/// @param queue [cl_command_queue]
/// @param blocking_event [cl_bool]
/// @param num_events_in_wait_list [cl_uint]
/// @param[in] event_wait_list [const cl_event*]
/// @param[out] event [cl_event*]
#define ae2fCL_AnnSpPredictB(_this, in, in_idx, outbuff_optional, context, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSpPredict( \
    _this, in, (cl_mem)0, in_idx, 0, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, context\
)

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Predict the output.
/// 
/// See [ae2fCL_AnnSpPredict]
/// 
/// # Memory Object Auto Allocate
///
/// It will allocate a memory object automatically for [in]. 
/// @param[out] _this 
/// @param[in] in 
/// The input vector in a length of [_this->mgWeightLen].
/// @param[out] out
/// Pointer to use as a buffer for as length as [ae2f_float_t].
/// When set to zero, the final output will be gone.
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @param context
/// The OpenCL Context in order to allocate the new buffer when needed. \n
/// Passing zero here will cause undefined behaviour. 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSpPredictBuffAuto(
    const ae2f_struct ae2fCL_AnnSp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
) noexcept;

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Manages to correct the weight and bias for expected output value [goal].
/// 
/// This function will call @ref ae2fCL_AnnSpPredict in order to get the output.
///
/// @param[in, out] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// @param[out] out_optionalA
/// Writable memory object to use as a buffer for as length as [ae2f_float_t].
/// When set to zero, it will allocate the new one.
/// @param in_idx Padding for [in].
/// @param out_idx_optionalA 
/// Paddings for [out_optionalA]. \n
/// Would not matter a thing when 0 has passed via [out_optionalA].
/// 
/// @param goal 
/// Expected (wanted) output.
/// @param learning_rate 
/// Learning rate. Will be multiplied with Loss.
/// @param[out] diff_ret_optional 
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// @param delta_precalculated_optionalC
/// When Delta has been pre-calculated somehow, you can utilise it via a pointer. \n
/// When not set to zero, it will try to read as an element of [ae2f_float_t].
/// 
/// Passing this argue non-null pointer could skip Prediction and skip the part of calculating the delta.
///
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event 
/// @param context_optionalB 
/// The OpenCL Context in order to allocate the new buffer when needed. 
ae2f_extern ae2f_SHAREDCALL 
ae2f_err_t ae2fCL_AnnSpTrain(
    ae2f_struct ae2fCL_AnnSp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,
    const ae2f_float_t* delta_precalculated_optionalC,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
) noexcept;

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Manages to correct the weight and bias for expected output value [goal].
/// 
/// See [ae2fCL_AnnSpTrain]
/// @param[in] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// @param[out] out
/// Writable memory object to use as a buffer for as length as [ae2f_float_t].
/// When set to zero, it will allocate the new one.
/// @param in_idx Padding for [in].
/// @param out_idx
/// Paddings for [out]. \n
/// Would not matter a thing when 0 has passed via [out_optionalA].
/// 
/// @param goal 
/// Expected (wanted) output.
/// @param learning_rate 
/// Learning rate. Will be multiplied with Loss.
/// @param[out] diff_ret_optional 
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event
#define ae2fCL_AnnSpTrainA(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSpTrain(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, 0, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, (cl_context)0)

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Manages to correct the weight and bias for expected output value [goal].
/// 
/// See [ae2fCL_AnnSpTrain]
/// @param[in] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// The readable byte size must be greater or same as ( [in_idx] + @ref ae2fCL_AnnSp::mgWeightLen ) * sizeof( @ref ae2f_float_t ).
/// @param in_idx Padding for [in].
/// 
/// @param goal 
/// Expected (wanted) output.
/// @param learning_rate 
/// Learning rate. Will be multiplied with Loss.
/// @param[out] diff_ret_optional 
/// The final output for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event
/// @param context
/// The OpenCL Context in order to allocate the new buffer when needed. 
#define ae2fCL_AnnSpTrainB(_this, in, in_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context) \
ae2fCL_AnnSpTrain(_this, in, (cl_mem)0, in_idx, 0, goal, learning_rate, diff_ret_optional, 0, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context)

/// @memberof ae2fCL_AnnSp
/// @brief 
/// # Manages to correct the weight and bias for expected output value [goal].
/// 
/// See [ae2fCL_AnnSpTrain]
/// 
/// Since goal and output(from @ref ae2fCL_AnnSpPredict) is for calculating the delta_precalculated,
/// so with given delta we could skip it all.
/// 
/// @param[in] _this 
/// @param[in] in 
/// The input value binded with an OpenCL memory object. \n
/// If you don't want to make a memory object manually, see [ae2fCL_AnnSpPredictBuffAuto].
/// 
/// @param in_idx Padding for [in].
/// @param learning_rate 
/// Learning rate. Will be multiplied with Loss.
/// @param[out] diff_ret_optional 
/// The final delta(loss) calculated for this operation would be stored here. \n 
/// Passing it zero will not cause undefined behaviour, but this operation's result is going nowhere.
///
/// @param delta_precalculated
/// When Delta has been pre-calculated somehow, you can utilise it via a pointer. \n
/// When not set to zero, it will try to read as an element of [ae2f_float_t], 
/// and skip the part of calculating the delta.
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param[out] event
/// @param context
/// The OpenCL Context in order to allocate the new buffer when needed. 
#define ae2fCL_AnnSpTrainC(_this, in, in_idx, learning_rate, diff_ret_optional, delta_precalculated, queue, blocking_read, num_events_in_wait_list, event_wait_list,  event) \
ae2fCL_AnnSpTrain(_this, in, 0, in_idx, 0, 0, learning_rate, diff_ret_optional, delta_precalculated, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, 0)

#define ae2fCL_AnnSp_NEED
#define ae2f_TMP

/// @brief 
/// # Single Perceptron.
/// 
/// This will read multiple inputs, generate one output.
typedef struct ae2fCL_AnnSp {

    /// @brief Bias.
    ae2f_float_t mBias;

    /// @brief Activasion function customisable.
    ae2fCL_fpAnnAct_t mAct;

    /// @brief
    /// It is a OpenCL Memory Object using a structure itself as a host memory. \n 
    /// Since ae2fCL_AnnSp is not completely defined yet, the type will be undefined.
    ae2fCL_memobj_void_t mSelf;

    /// @warning
    /// The length of the weight will be considered as same as the input's length for all cases.
    /// @brief
    /// It is a OpenCL Memory Object allocated alone as 
    ae2fCL_memobj_float_t mgWeight;

    /// @brief Specifies the way of calculating loss on training.
    /// @see ae2fCL_AnnSpTrain
    ae2fCL_fpAnnSpGetLoss_t mpGetLoss;

    /// @brief
    /// Possible count of weights, which means possible count of inputs on prediction.
    /// 
    /// @see ae2fCL_AnnSpPredict
    size_t mgWeightLen;


    #if ae2f_WhenCXX(1) + 0
    #include "Sp/Sp.hpp"
    #endif
} ae2fCL_AnnSp;


#if ae2f_WhenCXX(1) + 0
#include "Sp/imp.hpp"
#endif


#endif
#endif