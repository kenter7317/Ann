#include <ae2fCL/Ann/Mlp.h>
#include <memory.h>

ae2f_SHAREDEXPORT
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
) {
    ae2f_err_t _ = 0;
    cl_event* Events = event;
    size_t maxbuff = 0;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!layerLengths) return ae2f_errGlob_PTR_IS_NULL;
    _this->Count = --layerCount;
    _this->MaxBuff = 0;

    if(!Events && !(Events = malloc(layerCount)))
    return ae2f_errGlob_ALLOC_FAILED;
    #define return(code) {_ = code; goto END;} 

    
    if(!(_this->List = calloc(layerCount, sizeof(ae2fCL_AnnMlpEl))))
    return(ae2f_errGlob_ALLOC_FAILED);

    if(event_wait_list)
    clWaitForEvents(num_events_in_wait_list, event_wait_list);

    for(size_t i = 0; i < layerCount; i++) {
        _ |= ae2fCL_AnnSlpMk(
            _this->List + i,
            inputCounts_optional,
            padCount_optional,
            layerLengths[i],
            layerLengths[i + 1],
            mAct, fpGetLoss, ctx, queue, CL_FALSE,
            0, 0, Events + i
        );

        if(_this->MaxBuff < (maxbuff = _this->List[i].MaxInCount))
        _this->MaxBuff = maxbuff;

        if(_this->MaxBuff < (maxbuff = _this->List[i].OutCount))
        _this->MaxBuff = maxbuff;

        inputCounts_optional && (inputCounts_optional += layerLengths[i]);
        padCount_optional && (padCount_optional += layerLengths[i]);
    }

    if(blocking_read || !event)
    clWaitForEvents(layerCount, Events);

    END:
    #undef return
    if(Events && Events != event) free(Events);
    return _;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnMlpDel(
    ae2fCL_AnnMlp* _this
) {
    ae2f_err_t _ = 0;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;
    for(size_t i = 0; i < _this->Count; i++) {
        _ |= ae2fCL_AnnSlpDel(_this->List + i);
    }
    return _;
}

ae2f_SHAREDEXPORT
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
) {
    ae2f_err_t ret = 0;
    cl_int stateunderhood = CL_SUCCESS;
    cl_mem buffobj = buffobj_optionalA;
    ae2f_float_t
        * cache = outcache_optional;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    #define return(code) { ret |= code; goto RET; }

    if(!buffobj) {
        buffobj_idx_optionalA = 0;
        if(!context_optionalB) return ae2f_errGlob_PTR_IS_NULL;
        buffobj = clCreateBuffer(
            context_optionalB, 
            CL_MEM_READ_WRITE, 
            sizeof(ae2f_float_t) * (_this->MaxBuff + 1), 
            NULL, &stateunderhood
        );
        if(stateunderhood != CL_SUCCESS) return(ae2f_errGlob_ALLOC_FAILED);
    }

    if(!cache && !(cache = calloc(sizeof(ae2f_float_t), _this->MaxBuff)))
    return(ae2f_errGlob_ALLOC_FAILED);

    size_t i = 0;
    for(; i < _this->Count; i++) {
        ret |= ae2fCL_AnnSlpPredict(
            _this->List + i, 
            i ? buffobj : in,
            buffobj,
            i ? 0 : in_idx,
            _this->MaxBuff,
            cache,
            queue, CL_TRUE, 0, 0, 0,
            context_optionalB
        );

        stateunderhood = clEnqueueReadBuffer(
            queue, buffobj, CL_TRUE,
            0, sizeof(ae2f_float_t) * _this->List[i].OutCount,
            cache, 0, 0, 0
        );

        if(stateunderhood != CL_SUCCESS)
        ret |= ae2f_errGlob_NFOUND;
    }

    if(outret_optional) {
        memcpy(outret_optional, cache, _this->List[i].OutCount * sizeof(ae2f_float_t));
    }

    RET:
    if(buffobj && !buffobj_optionalA) 
    clReleaseMemObject(buffobj);
    if(cache) free(cache);

    #undef return
    return ret;
}


ae2f_SHAREDEXPORT
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
) {
    ae2f_err_t ret = 0;
    cl_int stateunderhood = CL_SUCCESS;
    ae2f_float_t
        * _cache = outcache_optional;
    cl_mem buffobj = buffobj_optionalA;

    const size_t
    // Width: one layer as float count multiplied 
    // ... skip for [_cache]
    PAD_DELTA_WFLOAT = _this->MaxBuff * sizeof(ae2f_float_t),

    // Skip all shit.
    SKIPALL = PAD_DELTA_WFLOAT * _this->Count;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    #define return(code) { ret |= code; goto RET; }

    if(!_cache && !(_cache = calloc(_this->Count, PAD_DELTA_WFLOAT << 1)))
    return(ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_PTR_IS_NULL);

    if(!buffobj) {
        buffobj_idx_optionalA = 0;
        if(!context_optionalB) return(ae2f_errGlob_PTR_IS_NULL);
        buffobj = clCreateBuffer(
            context_optionalB, 
            CL_MEM_READ_WRITE, 
            SKIPALL + sizeof(ae2f_float_t), 
            NULL, &stateunderhood
        );

        if(stateunderhood != CL_SUCCESS) 
        return(ae2f_errGlob_ALLOC_FAILED);
    }

    #pragma region forward operation customed.
    #define rreturn(code) { ret |= code; goto RRET; }
    {
        size_t i = 0; ae2f_float_t* cache = _cache;
        for(; i < _this->Count; i++) {
            const size_t
            IDX_OUT = PAD_DELTA_WFLOAT * i + buffobj_idx_optionalA,
            IDX_IN = IDX_OUT - PAD_DELTA_WFLOAT;

            ret |= ae2fCL_AnnSlpPredict(
                _this->List + i,
                i ? buffobj : in,
                buffobj,
                i ? IDX_IN : in_idx,
                SKIPALL,
                cache,
                queue, CL_TRUE, 0, 0, 0,
                context_optionalB
            );

            stateunderhood = clEnqueueWriteBuffer(
                queue, buffobj, CL_TRUE,
                IDX_OUT,
                sizeof(ae2f_float_t) * _this->List[i].OutCount,
                cache, 0, 0, 0
            );

            if(stateunderhood != CL_SUCCESS)
            ret |= ae2f_errGlob_NFOUND;
            cache += _this->MaxBuff;
        } RRET:
        if(ret) goto RET;
    }
    #undef rreturn
    #pragma endregion

    ae2f_float_t* const cachegoal = _cache + _this->MaxBuff;
    for(size_t i = _this->Count - 1; i != ((size_t)-1); i--) {
        const size_t
        IDX_OUT = PAD_DELTA_WFLOAT * i,
        IDX_IN = IDX_OUT - PAD_DELTA_WFLOAT;

        
    }

    RET:
    #undef return
    if(_cache && _cache != outcache_optional) 
    free(_cache);
    if(buffobj && !buffobj_optionalA) 
    clReleaseMemObject(buffobj);

    return ret;
}