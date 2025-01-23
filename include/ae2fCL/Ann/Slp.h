/// @file Slp.h
#ifndef ae2fCL_AnnSlp_h
#define ae2fCL_AnnSlp_h

#include "Sp.h"

/// @brief 
typedef struct ae2fCL_AnnSlpEl {
    size_t InputIdxPad;
    ae2fCL_AnnSp Perceptron[1];
} ae2fCL_AnnSlpEl;

/// @brief 
typedef struct ae2fCL_AnnSlp {
    ae2fCL_AnnSlpEl* List;
    size_t OutCount, MaxInCount;
} ae2fCL_AnnSlp;

/// @memberof ae2fCL_AnnSlp
#define ae2fCL_AnnSlpGetAct(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mAct)

/// @memberof ae2fCL_AnnSlp
#define ae2fCL_AnnSlpGetLoss(_this, i) (ae2f_const_cast(ae2fCL_AnnSlp*, _this)->List[i].Perceptron->mpGetLoss)

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// @param _this 
/// @param inputsCount_optionalA 
/// @param padCount_optional 
/// @param inputsCountGlobal_optionalB 
/// @param outputCount 
/// @param mAct 
/// @param fpGetLoss 
/// @param ctx 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param event_wait_list 
/// @param event 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpMk(
    ae2fCL_AnnSlp* _this,
    const size_t* inputsCount_optionalA,
    const size_t* padCount_optional,
    size_t inputsCountGlobal_optionalB,
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
/// @param _this 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnSlpDel(
    ae2fCL_AnnSlp* _this
) noexcept;

/// @memberof ae2fCL_AnnSlp
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
) noexcept;

/// @memberof ae2fCL_AnnSlp
/// @brief 
/// @param _this 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param in_idx 
/// @param out_idx_optionalA 
/// @param goal 
/// @param LearningRateGlobal_optional_A 
/// @param LearningRateArr_optional_B 
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

    const ae2f_float_t* goal,
    ae2f_float_t LearningRateGlobal_optional_A,
    ae2f_float_t* LearningRateArr_optional_B,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
) noexcept;

#endif