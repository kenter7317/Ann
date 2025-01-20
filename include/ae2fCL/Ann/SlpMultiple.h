/// @file SlpMultiple.h

#ifndef ae2fCL_AnnSlpMultiple_h
#define ae2fCL_AnnSlpMultiple_h

#include "Slp.h"

typedef struct ae2fCL_AnnSlpMultipleEl {
    size_t InputIdxPad;
    ae2fCL_AnnSlp Perceptron[1];
} ae2fCL_AnnSlpMultipleEl;

typedef struct ae2fCL_AnnSlpMultiple {
    ae2fCL_AnnSlpMultipleEl* List;
    size_t OutCount, MaxInCount;
} ae2fCL_AnnSlpMultiple;

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMultipleMk(
    ae2fCL_AnnSlpMultiple* _this,
    size_t* inputsCount_optionalA,
    size_t* padCount_optional,
    size_t inputsCountGlobal_optionalB,
    size_t outputCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSlpGetLoss_t fpGetLoss,
    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
);

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMultipleDel(
    ae2fCL_AnnSlpMultiple* _this
);

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMultiplePredict(
    ae2fCL_AnnSlpMultiple* _this,
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

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMultipleTrain(
    ae2fCL_AnnSlpMultiple* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    ae2f_float_t goal,
    ae2f_float_t LearningRateGlobal_optional_A,
    ae2f_float_t* LearningRateArr_optional_B,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
);

#endif