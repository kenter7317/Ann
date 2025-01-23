#include <ae2fCL/Ann/Sp.h>
#include <ae2f/errGlob.h>

#include <ae2fCL/Ann/LcgRand.h>
#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>

#include <stdio.h>

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnSpDel(
    ae2fCL_AnnSp* _this
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(_this->mgWeight) clReleaseMemObject(_this->mgWeight);
    if(_this->mSelf) clReleaseMemObject(_this->mSelf);
    _this->mgWeightLen = 0;
    _this->mgWeight = 0;
    _this->mBias = 0;
    _this->mAct = 0;
    _this->mSelf = 0;
    _this->mpGetLoss = 0;
    return ae2f_errGlob_OK;
}

static ae2f_float_t __LOSS_DEFAULT(ae2f_float_t out, ae2f_float_t goal) {
    return goal - out;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnSpMk(
    ae2f_struct ae2fCL_AnnSp* _this,
    const ae2f_float_t* inputs,
    size_t inputsCount,
    ae2fCL_fpAnnAct_t mAct,
    ae2fCL_fpAnnSpGetLoss_t fpGetLoss,

    cl_context ctx,
    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {
    const cl_kernel K = ae2fCL_AnnKerns[
        ae2fCL_eAnnKernsSpMkRand
    ];
    cl_int err;
    if(!_this)
    return ae2f_errGlob_PTR_IS_NULL;
    uint64_t got = ae2fCL_AnnLcgRandG();

    _this->mpGetLoss = fpGetLoss;
    if(!_this->mpGetLoss) {
        _this->mpGetLoss = __LOSS_DEFAULT;
    }
    _this->mBias = ae2fCL_AnnLcgRandDistribute(got);
    _this->mAct = mAct;
    _this->mgWeight = clCreateBuffer(
        ctx, CL_MEM_READ_WRITE, 
        inputsCount * sizeof(ae2f_float_t), 
        0, &err
    );
    _this->mgWeightLen = inputsCount;
    
    _this->mSelf = clCreateBuffer(
        ctx, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(ae2fCL_AnnSp),
        _this, &err
    );
    _this->mgWeightLen = inputsCount;
    if(err) return ae2f_errGlob_ALLOC_FAILED;
    if(inputs) {
        err = clEnqueueWriteBuffer(
            queue, _this->mgWeight, blocking_read, 0, sizeof(ae2f_float_t) * inputsCount, inputs, 
            num_events_in_wait_list, event_wait_list, event
        );

        if(err) return ae2f_errGlob_NFOUND;
        return ae2f_errGlob_OK;
    }

    err = clSetKernelArg(
        K, 0, sizeof(cl_mem),
        &_this->mgWeight
    );
    if(err) return ae2f_errGlob_NFOUND;
    
    #if cl_mem_SIZE == 8
        err = clSetKernelArg(
            K, 1,
            sizeof(uint32_t),
            ae2fCL_AnnLcgRandSeed.u32
        );
        if(err) return ae2f_errGlob_NFOUND;

        err = clSetKernelArg(
            K, 2,
            sizeof(uint32_t),
            ae2fCL_AnnLcgRandSeed.u32 + 1
        );
        if(err) return ae2f_errGlob_NFOUND;
    #else
        err = clSetKernelArg(
            K, 1,
            sizeof(uint64_t),
            &ae2fCL_AnnLcgRandSeed.u64
        );
        if(err) return ae2f_errGlob_NFOUND;
    #endif

    err = clEnqueueNDRangeKernel(
        queue, K,
        1, 0, &inputsCount, 0,
        num_events_in_wait_list, 
        event_wait_list, event
    );
    if(err) return ae2f_errGlob_NFOUND;
    ae2fCL_AnnLcgRandSeed.u64 = got;
    if(blocking_read && event) {
        clWaitForEvents(1, event);
        *event = 0;
    }

    return ae2f_errGlob_OK;
}


ae2f_SHAREDEXPORT 
ae2f_err_t ae2fCL_AnnSpPredict(
    const ae2fCL_AnnSp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx,
    ae2f_float_t* outbuff_optional_,
    cl_command_queue queue,
    cl_bool blocking_event,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context_optionalB
) {
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    const cl_kernel K = ae2fCL_AnnKerns[
        ae2fCL_eAnnKernsSpPredict
    ];

    cl_mem out = 0;
    cl_int err = CL_SUCCESS;
    ae2f_err_t errret = ae2f_errGlob_OK;
    ae2f_float_t outbuff = 0;
    #define Throw(code) { errret = code; goto __RET; }

    if(out_optionalA) {
        out = out_optionalA;
    } else {
        out_idx = 0;
        out = clCreateBuffer(
            context_optionalB, CL_MEM_READ_WRITE, 
            ae2f_float_t_SIZE, 0, &err
        ); if(err) Throw(ae2f_errGlob_ALLOC_FAILED);
    }

    err = clSetKernelArg(
        K, 0, sizeof(cl_mem),
        &_this->mSelf
    ); if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 1, sizeof(cl_mem),
        &out
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 2, sizeof(cl_mem),
        &_this->mgWeight
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 3, 
        sizeof(cl_mem),
        &in
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 4, 
        sizeof(ae2f_float_t) * _this->mgWeightLen,
        0
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    #if cl_mem_SIZE == 4
    err = clSetKernelArg(
        K, 5, 2, &out_idx
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 6, 2, ((const uint16_t*)(&out_idx)) + 1
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 7, 2, &in_idx
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 8, 2, ((const uint16_t*)(&in_idx)) + 1
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);
    #else
    err = clSetKernelArg(
        K, 5, 4, &out_idx
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);

    err = clSetKernelArg(
        K, 6, 4, &in_idx
    );
    if(err != CL_SUCCESS)
    Throw(ae2f_errGlob_WRONG_OPERATION);
    #endif

    err = clEnqueueNDRangeKernel(
        queue, K,
        1, 0, 
        &_this->mgWeightLen,
        &_this->mgWeightLen,
        0, 0, 0
    );
    if(err) Throw(ae2f_errGlob_NFOUND);

    err = clEnqueueReadBuffer(
        queue, out, blocking_event, 0, ae2f_float_t_SIZE, &outbuff, 
        num_events_in_wait_list, 
        event_wait_list, event
    ); if(err) Throw(ae2f_errGlob_NFOUND);

    outbuff += _this->mBias;
    if(_this->mAct) {
        outbuff = _this->mAct(outbuff);
    }

    if(outbuff_optional_) {
        outbuff_optional_[0] = outbuff;
    }

    __RET:
    #undef Throw
    if(out && out != out_optionalA) {
        clReleaseMemObject(out);
    }
    return errret;
}

#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>

ae2f_SHAREDEXPORT 
ae2f_err_t ae2fCL_AnnSpTrain(
    ae2fCL_AnnSp* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optionalA,
    uint32_t in_idx,
    uint32_t out_idx_optionalA,

    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optionalB
) {
    ae2f_float_t outF;
    ae2f_err_t err2 = ae2f_errGlob_OK;
    cl_event evbuff = 0;
    cl_mem out = out_optionalA;
    cl_int err = CL_SUCCESS;
    cl_kernel K = ae2fCL_AnnKerns[ae2fCL_eAnnKernsSpTrain];
    
    if(!diff_ret_optional) diff_ret_optional = &outF;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!out) {
        if(!context_optionalB)
        return ae2f_errGlob_PTR_IS_NULL;
        out = clCreateBuffer(
            context_optionalB,
            CL_MEM_WRITE_ONLY, 
            ae2f_float_t_SIZE,
            0, &err
        ); if(!out || err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
        out_idx_optionalA = 0;
    }

    err2 = ae2fCL_AnnSpPredictA(
        _this, in, out,
        in_idx, out_idx_optionalA, &outF,
        queue, CL_TRUE,
        0, 0, 0
    );
    if(err2) goto END;

    *diff_ret_optional = _this->mpGetLoss(outF, goal) * learning_rate;
    _this->mBias += (*diff_ret_optional);

    err = clSetKernelArg(K, 0, cl_mem_SIZE, &in);
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }

    err = clSetKernelArg(
        K, 1, cl_mem_SIZE, &_this->mgWeight
    );
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }
    #if ae2f_float_t_SIZE == cl_mem_SIZE
        const size_t Half_CL_Mem = cl_mem_SIZE >> 1;
        err = clSetKernelArg(
            K, 2, Half_CL_Mem, 
            diff_ret_optional
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }

        err = clSetKernelArg(
            K, 3, Half_CL_Mem,
            ((const char*)diff_ret_optional) + Half_CL_Mem
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }

        #define __KERARGLOC 4
    #else
        err = clSetKernelArg(
            K, 2, ae2f_float_t_SIZE, 
            diff_ret_optional
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }

        #define __KERARGLOC 3
    #endif

    #if cl_mem_SIZE == 4
        err = clSetKernelArg(
            K, __KERARGLOC, 2,
            &in_idx
        ); if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }

        err = clSetKernelArg(
            K, __KERARGLOC + 1, 2,
            (const uint16_t*)(&in_idx) + 1
        ); if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
    #else
        err = clSetKernelArg(
            K, __KERARGLOC, 4,
            &in_idx
        ); if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
    #endif

    err = clEnqueueNDRangeKernel(
        queue, K, 1, 0, &_this->mgWeightLen,
        0, 
        num_events_in_wait_list, 
        event_wait_list, event
    );
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }

    END:
    if(!out && out != out_optionalA) {
        clReleaseMemObject(out);
    }
    if(blocking_read && event) {
        clWaitForEvents(1, event);
    }
    return err2;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnSpPredictBuffAuto(
    const ae2fCL_AnnSp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out,

    cl_command_queue queue,
    cl_bool blocking_read,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,
    cl_context context
) {
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!out) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;
    cl_int err;
    ae2f_err_t E = ae2f_errGlob_OK;
    cl_mem inbuff;

    inbuff = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        _this->mgWeightLen * ae2f_float_t_SIZE,
        0, &err
    ); if(err) {
        E = ae2f_errGlob_ALLOC_FAILED;
        goto END;
    }

    err = clEnqueueWriteBuffer(
        queue, inbuff, CL_TRUE, 0,
        _this->mgWeightLen * ae2f_float_t_SIZE,
        in, 0, 0, 0
    ); if(err) {
        E = ae2f_errGlob_ALLOC_FAILED;
        goto END;
    }

    E = ae2fCL_AnnSpPredictB(
        _this, inbuff, 0, out, context, queue, 
        blocking_read, num_events_in_wait_list, 
        event_wait_list, event
    ); if(err) {
        E = ae2f_errGlob_NFOUND;
        goto END;
    }

    END:
    if(inbuff) clReleaseMemObject(inbuff);
    return E;
}