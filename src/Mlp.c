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
    cl_event* Events = event, *Events_For;
    size_t maxbuff = 0;
    ae2f_float_t evcount = 1;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!layerLengths) return ae2f_errGlob_PTR_IS_NULL;
    _this->Count = --layerCount;
    _this->MaxBuffCount = 0;

    for(size_t i = 0; i < layerCount; i++) {
        evcount *= layerLengths[i];
    }

    if(!Events && !(Events_For = Events = calloc(evcount, sizeof(cl_event))))
    return ae2f_errGlob_ALLOC_FAILED;
    #define return(code) {_ = code; goto END;} 

    
    if(!(_this->List = calloc(layerCount, sizeof(ae2fCL_AnnMlpEl))))
    return(ae2f_errGlob_ALLOC_FAILED);

    if(event_wait_list)
    clWaitForEvents(num_events_in_wait_list, event_wait_list);

    for(size_t i = 0; i < layerCount; i++) {
        #if 1
        _ |= ae2fCL_AnnSlpMk(
            _this->List + i,
            inputCounts_optional,
            padCount_optional,
            layerLengths[i],
            layerLengths[i + 1],
            mAct, fpGetLoss, ctx, queue, 
            CL_FALSE,
            0, 0, Events_For
        );

        if(_this->MaxBuffCount < (maxbuff = _this->List[i].MaxInCount))
        _this->MaxBuffCount = maxbuff;

        if(_this->MaxBuffCount < (maxbuff = _this->List[i].OutCount))
        _this->MaxBuffCount = maxbuff;
        #endif

        inputCounts_optional && (inputCounts_optional += layerLengths[i]);
        padCount_optional && (padCount_optional += layerLengths[i]);

        Events_For += layerLengths[i];
    }

    if(blocking_read || !event)
    clWaitForEvents(evcount, Events);

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
    free(_this->List);
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
            sizeof(ae2f_float_t) * (_this->MaxBuffCount + 1), 
            NULL, &stateunderhood
        );
        if(stateunderhood != CL_SUCCESS) return(ae2f_errGlob_ALLOC_FAILED);
    }

    if(!cache && !(cache = calloc(sizeof(ae2f_float_t), _this->MaxBuffCount)))
    return(ae2f_errGlob_ALLOC_FAILED);

    size_t i = 0;
    for(; i < _this->Count; i++) {
        ret |= ae2fCL_AnnSlpPredict(
            _this->List + i, 
            i ? buffobj : in,
            buffobj,
            i ? 0 : in_idx,
            _this->MaxBuffCount,
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

/// @todo
/// Error handeling is needed
/// @brief 
/// # Error Calculating Funtion for Hidden layer
/// 
/// Sum for multiplying Delta and Weight 
static ae2f_float_t MlpTrain_HidErr(
    const ae2fCL_AnnSlp* layerNxt,
    const ae2f_float_t* deltasNxt,
    size_t idxThen,
    cl_command_queue queue
) {
    ae2f_float_t ret = 0;
    for(size_t i = 0; i < layerNxt->OutCount; i++) {
        ae2f_float_t V;

        clEnqueueReadBuffer(
            queue, 
            layerNxt->List[i].Perceptron->mgWeight, 
            CL_TRUE, 0, sizeof(ae2f_float_t) * idxThen, 
            &V, 0, 0, 0
        );

        ret += V * deltasNxt[i];
    }
    return ret;
}

static void MlpTrain_HidCompute(
    const ae2fCL_AnnSlp* layerThen,
    const ae2fCL_AnnSlp* layerNxt,
    ae2f_float_t* retDeltaThen,
    ae2f_float_t* retGoalThen,
    const ae2f_float_t* deltasNxt,
    const ae2f_float_t* outThen,
    cl_command_queue queue
) {
    for(size_t i = 0; i < layerThen->OutCount; i++) {
        const ae2f_float_t err = MlpTrain_HidErr(layerNxt, deltasNxt, i, queue);
        retDeltaThen[i] = layerThen->List[i].Perceptron->mpGetLoss(
            outThen[i], 
            retGoalThen[i] = err + outThen[i]
        );
    }
}

static void MlpTrain_OutCompute(
    const ae2fCL_AnnSlp* layerOut,
    const ae2f_float_t* goal,
    const ae2f_float_t* out,
    ae2f_float_t* retDeltaOut
) {
    for(size_t i = 0; i < layerOut->OutCount; i++) {
        retDeltaOut[i] = layerOut->List[i].Perceptron->mpGetLoss(
            out[i], goal[i]
        );
    }
}

#include <stdio.h>

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

    // out and goal
    ae2f_float_t
        * _cache = outcache_optional;
    cl_mem buffobj = buffobj_optionalA;

    const size_t
    // Width: one layer as float count multiplied 
    // ... skip for [_cache]
    // 
    // _this->MaxBuffCount * sizeof(ae2f_float_t)
    PAD_DELTA_WFLOAT = _this->MaxBuffCount * sizeof(ae2f_float_t),

    // Skip all shit.
    SKIPALL = PAD_DELTA_WFLOAT * _this->Count;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->List) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    #define return(code) { ret |= code; goto RET; }

    // HERE. ALLOC IS FAILING
    if(!_cache && !(_cache = calloc(PAD_DELTA_WFLOAT, (_this->Count << 1) + 1))) {
        puts("Alloc failed");
        printf("%p\n", _cache);
        return(ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_PTR_IS_NULL);
    }

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

            if(i == _this->Count - 1 && outret_optional) {
                memcpy(outret_optional, cache, sizeof(ae2f_float_t) * _this->List[i].OutCount);
            }

            if(stateunderhood != CL_SUCCESS)
            ret |= ae2f_errGlob_NFOUND;
            cache += _this->MaxBuffCount;
        } RRET:
        if(ret) goto RET;
    }
    #undef rreturn
    #pragma endregion

    ae2f_float_t
    * const cachedelta = _cache + _this->MaxBuffCount,
    * const cachegoal = _cache + (_this->MaxBuffCount << 1);
    
    for(size_t i = _this->Count - 1; i != ((size_t)-1); i--) {
        const size_t CACHEIDXCHUNK = _this->MaxBuffCount * i;
        const size_t CACHEIDXCHUNKNXT = _this->MaxBuffCount * (i + 1);
        ae2fCL_AnnSlp* const Layer = _this->List + i;

        const size_t
        IDX_OUT = PAD_DELTA_WFLOAT * i + buffobj_idx_optionalA,
        IDX_IN = IDX_OUT - PAD_DELTA_WFLOAT;
        
        if(_this->Count - 1 == i) {
            MlpTrain_OutCompute(
                Layer, 
                goal, 
                _cache + CACHEIDXCHUNK, 
                cachedelta + CACHEIDXCHUNK
            );

            ret |= ae2fCL_AnnSlpTrain(
                Layer,
                buffobj,
                i ? buffobj : in,
                i ? IDX_IN : in_idx,
                SKIPALL,
                goal, LearningRate, 0, 
                0, queue, 0, 0, 0, 0, context_optionalB
            );
        } else {
            MlpTrain_HidCompute(
                Layer,
                Layer + 1,
                cachedelta + CACHEIDXCHUNK,
                cachegoal,
                cachedelta + CACHEIDXCHUNKNXT,
                _cache + CACHEIDXCHUNK,
                queue
            );

            ret |= ae2fCL_AnnSlpTrain(
                Layer,
                buffobj,
                i ? buffobj : in,
                i ? IDX_IN : in_idx,
                SKIPALL,
                cachegoal, LearningRate, 0, 
                0, queue, 0, 0, 0, 0, context_optionalB
            );
        }
    }

    RET:
    #undef return
    if(_cache && _cache != outcache_optional) 
    free(_cache);
    if(buffobj && !buffobj_optionalA) 
    clReleaseMemObject(buffobj);

    return ret;
}