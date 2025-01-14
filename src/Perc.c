#include <ae2fCL/Ann/Perc.h>
#include <ae2f/errGlob.h>

#include <ae2fCL/Ann/LcgRand.h>
#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnPercDel(
    ae2fCL_AnnPerc* _this
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(_this->mg_field) clReleaseMemObject(_this->mg_field);
    if(_this->self) clReleaseMemObject(_this->self);
    _this->mg_fieldLen = 0;
    _this->mg_field = 0;
    _this->m_bias = 0;
    _this->act = 0;
    _this->self = 0;
    _this->mfp_GetLoss = 0;
    return ae2f_errGlob_OK;
}


static ae2f_float_t __LOSS_DEFAULT(ae2f_float_t out, ae2f_float_t goal) {
    return goal - out;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnPercMk(
    ae2f_struct ae2fCL_AnnPerc* _this,
    const ae2f_float_t* inputs,
    size_t inputsCount,
    ae2fCL_efAnnAct_t act,
    ae2fCL_fpAnnPercGetLoss_t fpGetLoss,

    cl_context ctx,
    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {
    const cl_kernel K = ae2fCL_AnnKerns[
        ae2fCL_eAnnKernsPercMkRand
    ];
    cl_int err;
    if(!_this)
    return ae2f_errGlob_PTR_IS_NULL;
    uint64_t got = ae2fCL_AnnLcgRandG();

    _this->mfp_GetLoss = fpGetLoss;
    if(!_this->mfp_GetLoss) {
        _this->mfp_GetLoss = __LOSS_DEFAULT;
    }
    _this->m_bias = ae2fCL_AnnLcgRandDistribute(got);
    _this->act = act;
    _this->mg_field = clCreateBuffer(
        ctx, CL_MEM_READ_WRITE, 
        inputsCount * sizeof(ae2f_float_t), 
        0, &err
    );
    _this->mg_fieldLen = inputsCount;
    
    _this->self = clCreateBuffer(
        ctx, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(ae2fCL_AnnPerc),
        _this, &err
    );
    _this->mg_fieldLen = inputsCount;
    if(err) return ae2f_errGlob_ALLOC_FAILED;
    if(inputs) {
        err = clEnqueueWriteBuffer(
            queue, _this->mg_field, CL_TRUE, 0, sizeof(ae2f_float_t) * inputsCount, inputs, 
            num_events_in_wait_list, event_wait_list, event
        );

        if(err) return ae2f_errGlob_NFOUND;
        return ae2f_errGlob_OK;
    }

    err = clSetKernelArg(
        K, 0, sizeof(cl_mem),
        &_this->mg_field
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
    return ae2f_errGlob_OK;
}


ae2f_SHAREDEXPORT 
ae2f_err_t ae2fCL_AnnPercPredict(
    ae2fCL_AnnPerc* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out,
    uint32_t in_idx,
    uint32_t out_idx,

    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!out) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    const cl_kernel K = ae2fCL_AnnKerns[
        ae2fCL_eAnnKernsPercPredict
    ];

    cl_int err = CL_SUCCESS;
    err = clSetKernelArg(
        K, 0, sizeof(cl_mem),
        &_this->self
    );
    if(err != CL_SUCCESS) 
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 1, sizeof(cl_mem),
        &out
    );
    if(err != CL_SUCCESS) 
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 2, sizeof(cl_mem),
        &_this->mg_field
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 3, 
        sizeof(cl_mem),
        &in
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 4, 
        sizeof(ae2f_float_t) * _this->mg_fieldLen,
        0
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    #if cl_mem_SIZE == 4
    err = clSetKernelArg(
        K, 5, 2, &out_idx
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 6, 2, ((const uint16_t*)(&out_idx)) + 1
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 7, 2, &in_idx
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 8, 2, ((const uint16_t*)(&in_idx)) + 1
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;
    #else
    err = clSetKernelArg(
        K, 5, 4, &out_idx
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 6, 4, &in_idx
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;
    #endif

    err = clEnqueueNDRangeKernel(
        queue, K,
        1, 0, 
        &_this->mg_fieldLen,
        &_this->mg_fieldLen,
        num_events_in_wait_list, 
        event_wait_list, event
    );
    if(err) return ae2f_errGlob_NFOUND;

    return ae2f_errGlob_OK;
}



#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>
ae2f_SHAREDEXPORT 
ae2f_err_t ae2fCL_AnnPercTrain(
    ae2fCL_AnnPerc* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) in,
    ae2fCL_HostPtr(__global, ae2f_float_t) out_optional,
    uint32_t in_idx,
    uint32_t out_idx,
    ae2f_float_t goal,
    ae2f_float_t learning_rate,
    ae2f_float_t* diff_ret_optional,

    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event,

    cl_context context_optional
) {
    ae2f_float_t outF, diff;
    ae2f_err_t err2 = ae2f_errGlob_OK;
    cl_event evbuff = 0;
    cl_mem out = out_optional;
    cl_int err = CL_SUCCESS;
    cl_kernel K = ae2fCL_AnnKerns[ae2fCL_eAnnKernsPercTrain];

    if(!diff_ret_optional) diff_ret_optional = &diff;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!out) {
        if(!context_optional)
        return ae2f_errGlob_PTR_IS_NULL;
        out = clCreateBuffer(
            context_optional,
            CL_MEM_WRITE_ONLY, sizeof(ae2f_float_t),
            0, &err
        );
        if(!out || err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
        out_idx = 0;
    }

    err2 = ae2fCL_AnnPercPredict(
        _this, in, out,
        in_idx, out_idx,
        queue, num_events_in_wait_list,
        event_wait_list, event
    );

    if(err2) goto END;
    err = clEnqueueReadBuffer(
        queue, out, CL_TRUE,
        out_idx * sizeof(ae2f_float_t),
        sizeof(ae2f_float_t), &outF, 1, event,
        &evbuff
    );

    union {
        ae2f_float_t whole;
        uint8_t CV[sizeof(ae2f_float_t)];
    } diff2 
    = {*diff_ret_optional = _this->mfp_GetLoss(outF, goal)};

    diff2.whole *= learning_rate;

    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }

    err = clSetKernelArg(K, 0, sizeof(cl_mem), in);
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }
    err = clSetKernelArg(
        K, 1, sizeof(cl_mem), &_this->mg_field
    );
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }
    #if ae2f_float_t_SIZE == cl_mem_SIZE
        const size_t Half_CL_Mem = sizeof(cl_mem) >> 1;
        err = clSetKernelArg(
            K, 2, Half_CL_Mem, 
            diff2.CV
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }

        err = clSetKernelArg(
            K, 3, Half_CL_Mem,
            diff2.CV + Half_CL_Mem
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
    #else
        err = clSetKernelArg(
            K, 2, sizeof(cl_mem), 
            diff2.CV
        );
        if(err) {
            err2 = ae2f_errGlob_NFOUND;
            goto END;
        }
    #endif

    err = clEnqueueNDRangeKernel(
        queue, K, 1, 0, &_this->mg_fieldLen,
        0, 1, &evbuff, event);
    if(err) {
        err2 = ae2f_errGlob_NFOUND;
        goto END;
    }

    END:
    if(!out && out != out_optional) {
        clReleaseMemObject(out);
    }

    return err2;
}