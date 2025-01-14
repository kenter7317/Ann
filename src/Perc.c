#include <ae2fCL/Ann/Perc.h>
#include <ae2f/errGlob.h>

#include <ae2fCL/Ann/LcgRand.h>
#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <stdio.h>

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnPercDel(
    ae2fCL_AnnPerc* _this
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!_this->mg_field) return ae2f_errGlob_PTR_IS_NULL;
    clReleaseMemObject(_this->mg_field);
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
    ae2fCL_fAnnPercAct_t act,
    cl_context ctx,
    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
) {

    #define K ae2fCL_AnnKerns[ae2fCL_eAnnKernsPercMkRand]
    #undef K
    #ifndef K
    const cl_kernel K = ae2fCL_AnnKerns[
        ae2fCL_eAnnKernsPercMkRand
    ];
    #endif
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