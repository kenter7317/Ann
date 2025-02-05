/// @file Mlp.h

#ifndef ae2fCL_Ann_Mlp_h
#define ae2fCL_Ann_Mlp_h

#include "Slp.h"

/// @brief 
/// # Multi-Layered Perceptron
///
/// It has a list of @ref ae2fCL_AnnSlp 
/// with some parameters for automation.
/// 
/// With calculating the delta with no goal,
/// so we don't need actual goal for hidden layers.
typedef struct ae2fCL_AnnMlp {
    /// @brief 
    /// List of single layer perceptron.
    ae2fCL_AnnSlp* List;
    
    size_t 
    /// @brief
    /// Count of the @ref ae2fCL_AnnMlp::List
    Count, 

    /// @brief
    /// Maximum count for buffer. (input & hidden).
    MaxBuffCount;
} ae2fCL_AnnMlp;

#define ae2fCL_AnnMlpGetAct(_this, i, j) (_this)->List[i].List[j].Perceptron->mAct
#define ae2fCL_AnnMlpGetLoss(_this, i, j) (_this)->List[i].List[j].Perceptron->mpGetLoss

/// @memberof ae2fCL_AnnMlp
/// @brief 
/// Initialise [_this] class. \n
/// Initialised class must pass @ref ae2fCL_AnnMlpDel in order to release the resources.
/// @param[out] _this 
/// @param[in] layerLengths 
/// Every layer's length. \n
/// Length of input and output are included.
/// 
/// @param[in] inputCounts_optional 
/// Counts of detailed input for every perceptron layer.
/// Its length will be a sum of [layerLengths].
/// 
/// @param[in] padCount_optional 
/// Padding count detailed for every perceptron.
/// 
/// @param layerCount 
/// Given count of the layer. \n
/// @ref ae2fCL_AnnMlp::Count will be set as this.
/// 
/// @param mAct 
/// Activasion function.
/// 
/// @param fpGetLoss 
/// Loss-calculation function.
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
/// Clear this class and any all members related.
/// @param[in, out] _this 
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
/// Predict the output for [in]. \n
/// Results will be stored at [outret_optional]
/// 
/// @param[in] _this 
/// @param[in] in 
/// OpenCL Memory Ojbect for input. \n
/// Its lenght is considered as @code _this->List[0].MaxInCount + in_idx
/// @param[out] buffobj_optionalA 
/// A buffer to store the temporary input. \n
/// Its length will be considered as @code _this->MaxBuffCount + buffobj_idx_optionalA
/// 
/// @param in_idx 
/// Padding index for [in].
/// 
/// @param buffobj_idx_optionalA 
/// Padding index for [buffobj_idx_optionalA].
/// @param[out] outret_optional 
/// Where the output data will be stored. \n
/// Its length will be considered as @code _this->List[_this->Count - 1].OutCount
/// 
/// @param[out] outcache_optional 
/// The cache to store the temporary output. \n
/// Its length will be @code _this->MaxBuffCount
/// 
/// @param queue 
/// @param context_optionalB 
/// OpenCL Context in a case of allocating the OpenCL Memory Object.
/// @return 
ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnMlpPredict(
    const ae2fCL_AnnMlp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t*) in,
    ae2fCL_HostPtr(__global, ae2f_float_t*) buffobj_optionalA,
    size_t in_idx,
    size_t buffobj_idx_optionalA,
    ae2f_float_t* outret_optional,
    ae2f_float_t* outcache_optional,
    cl_command_queue queue,
    cl_context context_optionalB
) noexcept;

/// @memberof ae2fCL_AnnMlp
/// @brief 
/// Train for output [goal] for given input of [in].
/// @param[in, out] _this 
/// @param[in] in 
/// OpenCL Memory Ojbect for input. \n
/// Its count is considered as @code _this->List[0].MaxInCount + in_idx
/// @param[out] buffobj_optionalA 
/// Its count is considered as @code _this->MaxBuffCount * _this->Count
/// 
/// @param in_idx 
/// @param buffobj_idx_optionalA 
/// @param out_idx_optionalA 
/// @param[out] outret_optional 
/// @param[out] outcache_optional 
/// @param[in] goal 
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