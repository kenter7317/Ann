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
    return ae2f_errGlob_OK;
}

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnPercMk(
    ae2f_struct ae2fCL_AnnPerc* _this,
    const ae2f_float_t* inputs,
    size_t inputsCount,
    ae2fCL_efAnnAct_t act,
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
    uint32_t idx,

    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {
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
        K, 5, 2, &idx
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    err = clSetKernelArg(
        K, 6, 2, ((const uint16_t*)(&idx)) + 1
    );
    if(err != CL_SUCCESS)
    return ae2f_errGlob_WRONG_OPERATION;

    #else
    err = clSetKernelArg(
        K, 5, 4, &idx
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



ae2f_SHAREDEXPORT 
ae2f_err_t ae2fCL_AnnPercTrain(
    ae2f_struct ae2fCL_AnnPerc* _this,
    ae2fCL_HostPtr(__global, ae2f_float_t) out,
    uint32_t idx, 

    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {

}