/// @file Perceptron
#ifndef ae2fCL_Ann_Perc_h
#define ae2fCL_Ann_Perc_h

#include <ae2f/Cast.h>
#include <ae2f/Call.h>
#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2f/errGlob.h>

#include "Act.h"

#include <ae2f/Pack/Beg.h>
typedef ae2f_float_t(*ae2fCL_fpAnnPercGetLoss_t)
    ae2fCL_whenC((ae2f_float_t out, ae2f_float_t goal));

typedef struct ae2fCL_AnnPerc {
    ae2f_float_t mBias;
    ae2fCL_fpAnnAct_t mAct;
    ae2fCL_HostPtr(__global, void) mSelf;
    ae2fCL_HostPtr(__global, ae2f_float_t) mgWeight;
    ae2fCL_fpAnnPercGetLoss_t mpGetLoss;
    size_t mgWeightLen;
} ae2fCL_AnnPerc;
#include <ae2f/Pack/End.h>

#ifndef ae2fCL_LocAsCL
ae2f_extern ae2f_SHAREDCALL ae2f_err_t ae2fCL_AnnPercMk(
    ae2f_struct ae2fCL_AnnPerc* _this,
    const ae2f_float_t* inputs,
    size_t inputsCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnPercGetLoss_t fpGetLoss,

    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
);

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnPercDel(
    ae2fCL_AnnPerc* _this
);

#define ae2fCL_AnnPercWeightCheck(_this, _out, queue) \
clEnqueueReadBuffer( \
      queue,  \
      (_this)->mgWeight, \
      CL_TRUE, 0, \
      sizeof(ae2f_float_t) * (_this)->mgWeightLen, \
      (_out), \
      0, 0, 0 \
)

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnPercPredict(
    ae2fCL_AnnPerc* _this,
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

#define ae2fCL_AnnPercPredictA(_this, in, out, in_idx, out_idx, outbuff_optional, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnPercPredict( \
    _this, in, out, in_idx, out_idx, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, 0\
)

#define ae2fCL_AnnPercPredictB(_this, in, in_idx, outbuff_optional, context, queue, blocking_event, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnPercPredict( \
    _this, in, (cl_mem)0, in_idx, 0, outbuff_optional, \
    queue, blocking_event, num_events_in_wait_list, event_wait_list, event, context\
)

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnPercPredictBuffAuto(
    ae2fCL_AnnPerc* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
);

/// @see ae2f_float_t
typedef union ae2f_floatvague ae2f_floatvague_t;

ae2f_extern ae2f_SHAREDCALL 
ae2f_err_t ae2fCL_AnnPercTrain(
    ae2fCL_AnnPerc* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_floatvague_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
);

#define ae2fCL_AnnPercTrainA(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event) \
ae2fCL_AnnPercTrain(_this, in, out, in_idx, out_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, (cl_context)0)

#define ae2fCL_AnnPercTrainB(_this, in, in_idx, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context) \
ae2fCL_AnnPercTrain(_this, in, (cl_mem)0, in_idx, 0, goal, learning_rate, diff_ret_optional, queue, blocking_read, num_events_in_wait_list, event_wait_list, event, context)


#endif
#endif