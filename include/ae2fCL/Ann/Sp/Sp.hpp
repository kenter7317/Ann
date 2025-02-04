/**
 * @file Sp.hpp
 * @author ae2f
 * @brief 
 * CXX Style Abstraction for @ref ae2fCL_AnnSp
 * 
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2025
 * 
 * @warning
 * This header is a utility for structure methods defining. \n
 * Being included outside a header is not predicted.
 * 
 * 
 */

#if !defined(ae2fCL_Ann_Sp_Sp_h)
#define ae2fCL_Ann_Sp_Sp_h

#include "../Sp.h"
#define ae2f_TMP

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
) const noexcept;

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
) const noexcept;

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
) const noexcept;

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
) noexcept;

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
) noexcept;

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
) noexcept;

inline cl_int ae2f_TMP WeightCheck(
    ae2f_float_t* buff, 
    cl_command_queue queue
) const noexcept;

#undef ae2f_TMP
#endif