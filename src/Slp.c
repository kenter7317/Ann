#include <ae2fCL/Ann/Slp.h>
#include <stdlib.h>
#include <omp.h>

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnSlpMk(
    ae2fCL_AnnSlp* _this,
    const size_t* inputsCount,
    const size_t* padCount,
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
) {
    ae2f_err_t _ = 0;
    #define return(code) {_ |= code; goto END;}
    cl_event* Events = event;

    size_t i = 0, maxbuff = 0;
    if(!_this) return(ae2f_errGlob_PTR_IS_NULL);
    _this->List = calloc(sizeof(ae2fCL_AnnSlpEl), outputCount);
    if(!_this->List) return(ae2f_errGlob_ALLOC_FAILED);
    if(!Events && !(Events = calloc(sizeof(cl_event), outputCount)));

    _this->OutCount = outputCount;
    _this->MaxInCount = inputsCountGlobal;
    if(event_wait_list)
    clWaitForEvents(num_events_in_wait_list, event_wait_list);

    __OutputCountIsProvided:
    for(i = 0; i < outputCount; i++) {
        _ |= ae2fCL_AnnSpMk(
            _this->List[i].Perceptron,
            0, 
            inputsCount ? inputsCount[i] : inputsCountGlobal, 
            mAct,
            fpGetLoss, ctx,
            queue, CL_FALSE, 0, 0, Events + i
        );

        size_t pad = 0;
        if(padCount)
        _this->List[i].InputIdxPad = pad = padCount[i];

        if(_this->MaxInCount < pad + (maxbuff = _this->List[i].Perceptron->mgWeightLen)) {
            _this->MaxInCount = maxbuff + pad;
        }
    }
    
    if(blocking_read || !event)
    clWaitForEvents(outputCount, Events);

    END:
    if(Events && Events != event) free(Events);
    #undef return
    return _;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnSlpDel(
    ae2fCL_AnnSlp* _this
) {
    ae2f_err_t _ = 0;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->OutCount; i++) {
        _ |= ae2fCL_AnnSpDel(_this->List[i].Perceptron);
        _this->List[i].InputIdxPad = 0;
    } free(_this->List);
    _this->List = 0;

    _this->OutCount = 0;
    return _;
}

ae2f_SHAREDEXPORT
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
) {
    ae2f_err_t ret = 0;
    cl_event* Events = event;
    #define return(code) {ret |= code; goto END;}
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;

    clWaitForEvents(num_events_in_wait_list, event_wait_list);
    
    if(
        !event && 
        !(Events = calloc(_this->OutCount, sizeof(cl_event)))
    ) return(ae2f_errGlob_ALLOC_FAILED);

    #pragma omp parallel for
    for(size_t i = 0; i < _this->OutCount; i++) {
        if(_this->MaxInCount > _this->List[i].InputIdxPad + _this->List[i].Perceptron->mgWeightLen)
        continue;

        ae2f_err_t _ret = ae2fCL_AnnSpPredict(
            _this->List[i].Perceptron, 
            in, out_optionalA, 
            _this->List[i].InputIdxPad + in_idx, 
            out_idx_optionalA,
            outbuff_optional_ ? outbuff_optional_ + i : 0, 
            queue, CL_TRUE, 0, 0, 
            Events + i, context_optionalB
        );

        if(_ret) {
            #pragma omp atomic
            ret |= _ret;
        }
    }
    
    if(blocking_event || !event) 
    clWaitForEvents(_this->OutCount, Events);

    END:
    #undef return
    if(Events && Events != event) free(Events);
    return ret;
}


ae2f_SHAREDEXPORT
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
) {
    ae2f_err_t ret = 0;
    #define return(code) {ret |= code; goto END;}

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;
    if(!goal) return ae2f_errGlob_PTR_IS_NULL;

    clWaitForEvents(num_events_in_wait_list, event_wait_list);

    for(size_t i = 0; i < _this->OutCount; i++) {
        ae2f_err_t _ret = ae2fCL_AnnSpTrain(
            _this->List[i].Perceptron,
            in, out_optionalA, 
            in_idx + _this->List[i].InputIdxPad,
            out_idx_optionalA, goal[i],
            LearningRateArr_optional_B ? LearningRateArr_optional_B[i] : LearningRateGlobal_optional_A,
            diff_ret_optional ? diff_ret_optional + i : 0, 
            queue, CL_TRUE,
            0, 0, 0, context_optionalB
        );

        if(_ret) {
            ret |= _ret;
        }
    }

    END:
    #undef return
    return ret;
}