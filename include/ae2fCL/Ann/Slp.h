/// @file Slpeptron
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
/// @return
/// Calculated loss.
typedef ae2f_float_t(*ae2fCL_fpAnnSlpGetLoss_t)
    ae2fCL_whenC((ae2f_float_t out, ae2f_float_t goal));

typedef ae2fCL_HostPtr(__global, void) ae2fCL_memobj_void_t;
typedef ae2fCL_HostPtr(__global, ae2f_float_t) ae2fCL_memobj_float_t;

/// @brief 
/// # Single Layer Perceptron.
/// 
/// This will read multiple inputs, generate one output.
typedef struct ae2fCL_AnnSlp {

    /// @brief Bias.
    ae2f_float_t mBias;

    /// @brief Activasion function customisable.
    ae2fCL_fpAnnAct_t mAct;

    /// @brief
    /// It is a OpenCL Memory Object using a structure itself as a host memory. \n 
    /// Since ae2fCL_AnnSlp is not completely defined yet, the type will be undefined.
    ae2fCL_memobj_void_t mSelf;

    /// @warning
    /// The length of the weight will be considered as same as the input's length for all cases.
    /// @brief
    /// It is a OpenCL Memory Object allocated alone as 
    ae2fCL_memobj_float_t mgWeight;

    /// @brief Specifies the way of calculating loss on training.
    /// @see ae2fCL_AnnSlpTrain
    ae2fCL_fpAnnSlpGetLoss_t mpGetLoss;

    /// @brief
    /// Possible count of weights, which means possible count of inputs on prediction.
    /// 
    /// @see ae2fCL_AnnSlpPredict
    size_t mgWeightLen;

} ae2fCL_AnnSlp;
#include <ae2f/Pack/End.h>

#ifndef ae2fCL_LocAsCL

/// @brief 
/// Initialises the class.
/// @param _this 
/// @param inputs_optional 
/// @param inputsCount 
/// @param mAct 
/// @param fpGetLoss 
/// @param ctx 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @return 
ae2f_extern ae2f_SHAREDCALL ae2f_err_t ae2fCL_AnnSlpMk(
    ae2f_struct ae2fCL_AnnSlp* _this,
    const ae2f_float_t* inputs_optional,
    size_t inputsCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSlpGetLoss_t fpGetLoss,

    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
);

/// @brief 
/// @param _this 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpDel(
    ae2fCL_AnnSlp* _this
);


#define ae2fCL_AnnSlpWeightCheck(_this, _out, queue) \
clEnqueueReadBuffer( \
      queue,  \
      (_this)->mgWeight, \
      CL_TRUE, 0, \
      sizeof(ae2f_float_t) * (_this)->mgWeightLen, \
      (_out), \
      0, 0, 0 \
)

/// @brief 
/// @param _this 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param in_idx 
/// @param out_idx_optionalA 
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
);

#define ae2fCL_AnnSlpPredictA(_this, in, out, in_idx, out_idx, outbuff_optional, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSlpPredict( \
    _this, in, out, in_idx, out_idx, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, 0\
)

#define ae2fCL_AnnSlpPredictB(_this, in, in_idx, outbuff_optional, context, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSlpPredict( \
    _this, in, (cl_mem)0, in_idx, 0, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, context\
)

/// @brief 
/// @param _this 
/// @param in 
/// @param out 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @param context 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpPredictBuffAuto(
    const ae2fCL_AnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
);

/// @brief 
/// @param _this 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param in_idx 
/// @param out_idx_optionalA 
/// @param goal 
/// @param learning_rate 
/// @param diff_ret_optional 
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

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
);

#define ae2fCL_AnnSlpTrainA(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnSlpTrain(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, (cl_context)0)

#define ae2fCL_AnnSlpTrainB(_this, in, in_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context) \
ae2fCL_AnnSlpTrain(_this, in, (cl_mem)0, in_idx, 0, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context)


#endif
#endif