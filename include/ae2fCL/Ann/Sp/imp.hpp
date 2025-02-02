/**
 * @file imp.hpp
 * @author ae2f
 * @brief 
 * @date 2025-02-01
 * 
 * @copyright Copyright (c) 2025
 * 
 * It contains the implementations of @ref ae2fCL_AnnSp.
 */

#if !defined(ae2fCL_Ann_Sp_imp_hpp)
#define ae2fCL_Ann_Sp_imp_hpp

#define ae2f_TMP ae2fCL_AnnSp::
#include "../Sp.h"

inline ae2f_err_t ae2f_TMP Predict(
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out,
    uint32_t in_idx,
    uint32_t out_idx,
    ae2f_float_t* outbuff_optional,
    cl_command_queue queue,
    cl_bool blocking_event,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) const noexcept {
    return ae2fCL_AnnSpPredictA(
        this, in, out, in_idx, out_idx, 
        outbuff_optional, queue, blocking_event, 
        num_events_in_wait_list, event_wait_list, event
    );
}

inline ae2f_err_t ae2f_TMP Predict(
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    uint32_t in_idx,
    ae2f_float_t* outbuff_optional,
    cl_command_queue queue,
    cl_bool blocking_event,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
) const noexcept {
    return ae2fCL_AnnSpPredictB(
        this, in, in_idx,
        outbuff_optional, context,
        queue, blocking_event, num_events_in_wait_list,
        event_wait_list, event  
    );
}

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
) const noexcept {
    return ae2fCL_AnnSpPredictBuffAuto(
        this, in, out, queue,
        blocking_read, num_events_in_wait_list,
        event_wait_list, event, context
    );
}

inline ae2f_err_t ae2f_TMP Train(
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out,
    uint32_t in_idx,
    uint32_t out_idx,

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) noexcept {
    return ae2fCL_AnnSpTrainA(
        this, in, out, in_idx, out_idx,
        goal, learning_rate, diff_ret_optional,
        queue, blocking_read, num_events_in_wait_list,
        event_wait_list, event
    );
}

inline ae2f_err_t ae2f_TMP Train(
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    uint32_t in_idx,

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context
) noexcept {
    return ae2fCL_AnnSpTrainB(
        this, in, in_idx, goal, learning_rate,
        diff_ret_optional, queue, blocking_read,
        num_events_in_wait_list, event_wait_list,
        event, context
    );
}

inline ae2f_err_t ae2f_TMP Train(
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    uint32_t in_idx,

    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,
    const ae2f_float_t* delta_precalculated,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) noexcept {
    return ae2fCL_AnnSpTrainC(
        this, in, in_idx, learning_rate, diff_ret_optional,
        delta_precalculated, queue, blocking_read, 
        num_events_in_wait_list, event_wait_list, event
    );
}

inline cl_int ae2f_TMP WeightCheck(
    ae2f_float_t* buff, 
    cl_command_queue queue
) const noexcept {
    return ae2fCL_AnnSpWeightCheck(this, buff, queue);
}


#endif
#undef ae2f_TMP