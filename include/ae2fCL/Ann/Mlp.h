/// @file Mlp.h

#ifndef ae2fCL_Ann_Mlp_h
#define ae2fCL_Ann_Mlp_h

#include "Slp.h"

/// @brief 
/// 
typedef struct ae2fCL_AnnMlp {
    /// @brief 
    ae2fCL_AnnSlp* List;
    
    size_t 
    /// @brief
    Count, 

    /// @brief
    MaxBuffCount;
} ae2fCL_AnnMlp;

#define ae2fCL_AnnMlpGetAct(_this, i, j) (_this)->List[i].List[j].Perceptron->mAct
#define ae2fCL_AnnMlpGetLoss(_this, i, j) (_this)->List[i].List[j].Perceptron->mpGetLoss

/// @memberof ae2fCL_AnnMlp
/// @brief 
/// Initialise this class. \n
/// 
/// @param[out] _this 
/// @param[in] layerLengths 
/// @param[in] inputCounts_optional 
/// @param[in] padCount_optional 
/// @param layerCount 
/// @param mAct 
/// @param fpGetLoss 
/// @param ctx 
/// @param queue 
/// @param blocking_read 
/// @param num_events_in_wait_list 
/// @param[in] event_wait_list 
/// @param event 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnMlpMk(
    ae2fCL_AnnMlp* _this,
    const size_t* layerLengths,
    const size_t* inputCounts_optional,
    const size_t* padCount_optional,
    size_t layerCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSpGetLoss_t fpGetLoss,
    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) noexcept;

/// @memberof ae2fCL_AnnMlp
/// @brief 
/// @param _this 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnMlpDel(
    ae2fCL_AnnMlp* _this
) noexcept;

/// @todo
/// Don't know why though Predict with pre-allocated OpenCL Memory Object is casting us an exception. \n
/// Perhaps using same memory object for input and output was a problem?
/// 
/// See related problem on @ref ae2fCL_AnnSlpPredict
/// @memberof ae2fCL_AnnMlp
/// @brief 
/// @param _this 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param in_idx 
/// @param buffobj_idx_optionalA 
/// @param outret_optional 
/// @param outcache_optional 
/// @param queue 
/// @param context_optionalB 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnMlpPredict(
    ae2fCL_AnnMlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t*) in,
    ae2fCL_HostPtr(__global, ae2f_float_t*) buffobj_optionalA,
    size_t in_idx,
    size_t buffobj_idx_optionalA,
    ae2f_float_t* outret_optional,
    ae2f_float_t* outcache_optional,
    cl_command_queue queue,
    cl_context context_optionalB
) noexcept;

/// @todo
/// Allocation is failing.
/// @memberof ae2fCL_AnnMlp
/// @brief 
/// @param _this 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param ae2f_float_t 
/// @param in_idx 
/// @param buffobj_idx_optionalA 
/// @param out_idx_optionalA 
/// @param outret_optional 
/// @param outcache_optional 
/// @param goal 
/// @param LearningRate 
/// @param queue 
/// @param context_optionalB 
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnMlpTrain(
    ae2fCL_AnnMlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t*) in,
    ae2fCL_HostPtr(__global, ae2f_float_t*) buffobj_optionalA,
    ae2fCL_HostPtr(__global, ae2f_float_t*) out_optionalA,
    size_t in_idx,
    size_t buffobj_idx_optionalA,
    size_t out_idx_optionalA,
    ae2f_float_t* outret_optional,
    ae2f_float_t* outcache_optional,
    const ae2f_float_t* goal,
    ae2f_float_t LearningRate,
    cl_command_queue queue,
    cl_context context_optionalB
) noexcept;

#endif