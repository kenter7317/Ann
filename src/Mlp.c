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

    const size_t SkipInput = _this->MaxBuffCount;

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
            sizeof(ae2f_float_t) * (SkipInput + 1), 
            NULL, &stateunderhood
        );
        if(stateunderhood != CL_SUCCESS) return(ae2f_errGlob_ALLOC_FAILED);
    }

    if(!cache && !(cache = calloc(sizeof(ae2f_float_t), SkipInput)))
    return(ae2f_errGlob_ALLOC_FAILED);

    size_t i;
    #pragma omp parallel for
    for(i = 0; i < _this->Count; i++) {
        #pragma omp atomic
        ret |= ae2fCL_AnnSlpPredict(
            _this->List + i, 
            i ? buffobj : in,
            buffobj,
            i ? 0 : in_idx,
            SkipInput,
            cache,
            queue, CL_TRUE, 0, 0, 0,
            context_optionalB
        );

        cl_int stateunderhood = clEnqueueWriteBuffer(
            queue, buffobj, CL_TRUE,
            0, sizeof(ae2f_float_t) * _this->List[i].OutCount,
            cache, 0, 0, 0
        );

        if(stateunderhood != CL_SUCCESS)
        #pragma omp atomic
        ret |= ae2f_errGlob_NFOUND;
    }

    if(outret_optional) {
        memcpy(outret_optional, cache, _this->List[i - 1].OutCount * sizeof(ae2f_float_t));
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

    #pragma omp parallel for
    for(size_t i = 0; i < layerNxt->OutCount; i++) {
        ae2f_float_t V;
        clEnqueueReadBuffer(
            queue, 
            layerNxt->List[i].Perceptron->mgWeight, 
            CL_TRUE, sizeof(ae2f_float_t) * idxThen, 
            sizeof(ae2f_float_t), 
            &V, 0, 0, 0
        );

        #pragma omp atomic
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
    #pragma omp parallel for
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
    #pragma omp parallel for
    for(size_t i = 0; i < layerOut->OutCount; i++) {
        retDeltaOut[i] = layerOut->List[i].Perceptron->mpGetLoss(
            out[i], goal[i]
        );
    }
}

static ae2f_err_t MlpTrain_Predict(
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

    const size_t SkipInput = _this->MaxBuffCount * _this->Count;

    #pragma omp parallel for
    for(size_t i = 0; i < _this->Count; i++) {
        ae2f_err_t _ret = ae2fCL_AnnSlpPredict(
            _this->List + i, 
            i ? buffobj : in,
            buffobj,
            i ? _this->MaxBuffCount * i : in_idx,
            SkipInput + buffobj_idx_optionalA,
            cache + i * _this->MaxBuffCount,
            queue, CL_TRUE, 0, 0, 0,
            context_optionalB
        );

        cl_int stateunderhood = clEnqueueWriteBuffer(
            queue, buffobj, CL_TRUE,
            (_this->MaxBuffCount * (i + 1)) * sizeof(ae2f_float_t) + buffobj_idx_optionalA, 
            sizeof(ae2f_float_t) * _this->List[i].OutCount,
            cache + i * _this->MaxBuffCount, 0, 0, 0
        );

        if(stateunderhood != CL_SUCCESS)
        _ret |= ae2f_errGlob_NFOUND;

        if(_ret) {
            #pragma omp atomic
            ret |= _ret;
        }
    }

    if(outret_optional) {
        memcpy(
            outret_optional, 
            cache, _this->List[_this->Count - 1].OutCount * sizeof(ae2f_float_t)
        );
    }

    RET:
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

    // out
    ae2f_float_t
        * _cache = outcache_optional;
    cl_mem buffobj = buffobj_optionalA;

    const size_t 
    MAXBUFFCOUNT_FOR_LAYER = _this->MaxBuffCount;
    const size_t SkipInput = _this->MaxBuffCount * _this->Count;

    if(!buffobj) {
        buffobj_idx_optionalA = 0;
        if(!context_optionalB) return ae2f_errGlob_PTR_IS_NULL;
        buffobj = clCreateBuffer(
            context_optionalB, 
            CL_MEM_READ_WRITE, 
            sizeof(ae2f_float_t) * (SkipInput + 1), 
            NULL, &stateunderhood
        );
        if(stateunderhood != CL_SUCCESS) return(ae2f_errGlob_ALLOC_FAILED);
    }

    #define _cache_ALLOCCOUNT SkipInput * 3
    #define _cache_SIZE sizeof(ae2f_float_t)
    if(!_cache) {
        _cache = calloc(_cache_SIZE, _cache_ALLOCCOUNT);
    }

    ae2f_float_t* const cache_Deltas = _cache + SkipInput;
    ae2f_float_t* const cache_Goals = cache_Deltas + SkipInput;


    ret |= MlpTrain_Predict(
        _this, in, buffobj, in_idx, 
        buffobj_idx_optionalA, outret_optional, 
        _cache, queue, context_optionalB
    ); if(ret) goto RET;

    for(size_t i = _this->Count - 1; i != ((size_t)-1); i--) {
        if(i == _this->Count - 1) {
            MlpTrain_OutCompute(
                _this->List + i,
                goal, _cache + i * _this->MaxBuffCount,
                cache_Deltas + i * _this->MaxBuffCount
            );

            ae2fCL_AnnSlpTrain(
                _this->List + i,
                i ? buffobj : in, 0, 
                _this->MaxBuffCount * i,
                0, goal, LearningRate, 0, 0, queue, 
                CL_TRUE,
                0, 0, 0, context_optionalB
            );
        } else {
            MlpTrain_HidCompute(
                _this->List + i,
                _this->List + i + 1,
                cache_Deltas + i * _this->MaxBuffCount,
                cache_Goals + i * _this->MaxBuffCount,
                cache_Deltas + (i + 1) * _this->MaxBuffCount,
                _cache + i * _this->MaxBuffCount,
                queue
            );

            ae2fCL_AnnSlpTrain(
                _this->List + i,
                i ? buffobj : in, 0,
                i ? _this->MaxBuffCount + i : in_idx,
                0, cache_Goals + i * _this->MaxBuffCount,
                LearningRate, 0, 0,
                queue, CL_TRUE, 0, 0, 0,
                context_optionalB
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