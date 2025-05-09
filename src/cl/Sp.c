#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Sp.h>
#include "CLCode/Size/ae2f_float_t.auto.h"
#include "CLCode/Size/cl_mem.auto.h"
#include <stdio.h>

static ae2f_mAnnSpPredict_t Predict;
static ae2f_mAnnSpTrain_t Train;
static ae2f_mAnnSpClean_t Clean;

static ae2f_err_t Predict(
    const ae2f_mAnnSp *_this, 
    const ae2f_float_t *in, 
    ae2f_float_t *outret_opt
) {
    ae2f_err_t err = 0;
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!outret_opt) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;
    if(ae2fCL_Ann.LErr) return ae2f_errGlob_NFOUND;

    cl_mem _W = *ae2fCL_mAnnSpWCl(_this, const), _IO = *ae2fCL_mAnnSpIOCl(_this, const);
    cl_kernel K = ae2fCL_Ann.Kerns[ae2fCL_eAnnKernsSpPredict];
    ae2f_float_t IBuffer;

    cl_event event[2] = {0, 0};

    #define return(n) { err = n; goto END; }

    ae2fCL_Ann.LErr = clEnqueueWriteBuffer(
        ae2fCL_Ann.Q, 
        _IO, CL_FALSE, 
        0, ae2f_float_t_SIZE * _this->inc, 
        in, 0, 0, event
    );

    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clSetKernelArg(K, 0, sizeof(cl_mem), &_IO);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clSetKernelArg(K, 1, sizeof(cl_mem), &_W);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clSetKernelArg(K, 2, _this->inc * sizeof(ae2f_float_t), 0);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    const size_t _locworksize = _this->inc;

    ae2fCL_Ann.LErr = clEnqueueNDRangeKernel(
        ae2fCL_Ann.Q, K, 1, 0,
        &_this->inc,
        &_locworksize,
        1, event, event + 1
    );

    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);
    ae2fCL_Ann.LErr = clReleaseEvent(event[0]); event[0] = 0;
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);

    ae2fCL_Ann.LErr = clEnqueueReadBuffer(
        ae2fCL_Ann.Q, _IO, 
        CL_FALSE, _this->inc * ae2f_float_t_SIZE, 
        ae2f_float_t_SIZE, &IBuffer, 
        1, event + 1, event
    );
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);
    ae2fCL_Ann.LErr = clReleaseEvent(event[1]); event[1] = 0;
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);

    if(event) ae2fCL_Ann.LErr = clWaitForEvents(1, event);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);
    if(event) ae2fCL_Ann.LErr = clReleaseEvent(event[0]); event[0] = 0;
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);

    printf("IBUFFER: %f\n", IBuffer);
    IBuffer = _this->vAct(IBuffer + *ae2f_mAnnSpB(_this, const));
    if(outret_opt) *outret_opt = IBuffer;

    END:
    if(event[0]) {
        ae2fCL_Ann.LErr = clWaitForEvents(1, event);
        if(!ae2fCL_Ann.LErr)
            ae2fCL_Ann.LErr = clReleaseEvent(event[0]);
        event[0] = 0;
    }

    if(ae2fCL_Ann.LErr) {
        return ae2f_errGlob_FLUSH_FAILED | ae2f_errGlob_NFOUND;
    }
    if(event[1]) {
        ae2fCL_Ann.LErr = clWaitForEvents(1, event + 1);
        if(!ae2fCL_Ann.LErr)
            ae2fCL_Ann.LErr = clReleaseEvent(event[1]);
        event[1] = 0;
    }

    if(ae2fCL_Ann.LErr) {
        return ae2f_errGlob_FLUSH_FAILED | ae2f_errGlob_NFOUND;
    }

    return err;
}

#include "./CLCode/uf.h"

static ae2f_err_t Train(
    ae2f_mAnnSp *_this, 
    const ae2f_float_t *in, 
    const ae2f_float_t *delta_optA, 
    ae2f_float_t goal_optB, 
    ae2f_float_t learningrate
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(learningrate == 0) return ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_WRONG_OPERATION;
    if(ae2fCL_Ann.LErr) return ae2f_errGlob_NFOUND;
    ae2f_err_t er = 0;

    cl_event event[2] = {0, 0};

    #undef return
    #define return(n) { er = n; goto END; }

    UF_t uf;
    if(delta_optA) 
        *uf.F = *delta_optA;
    else {
        er = (_this->vPredict ? _this->vPredict(_this, in, uf.F) : ae2f_errGlob_IMP_NOT_FOUND);
        *uf.F = _this->vLossDeriv(uf.F, &goal_optB, 0, 1) 
        * (_this->vActDeriv ? _this->vActDeriv(*uf.F) : *uf.F);
    }
    if(er) return er;

    *uf.F *= learningrate;
    *ae2f_mAnnSpB(_this) -= *uf.F;

    cl_mem _W = *ae2fCL_mAnnSpWCl(_this), _IO = *ae2fCL_mAnnSpIOCl(_this);
    cl_kernel K = ae2fCL_Ann.Kerns[ae2fCL_eAnnKernsSpTrain];
    ae2fCL_Ann.LErr = clSetKernelArg(K, 0, sizeof(cl_mem), &_IO);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clSetKernelArg(K, 1, sizeof(cl_mem), &_W);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    #if cl_mem_SIZE == ae2f_float_t_SIZE    
    ae2fCL_Ann.LErr = clSetKernelArg(K, 2, sizeof(ae2f_float_half_t), uf.UHalf);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;
    
    ae2fCL_Ann.LErr = clSetKernelArg(K, 3, sizeof(ae2f_float_half_t), uf.UHalf + 1);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;
    #else
    ae2fCL_Ann.LErr = clSetKernelArg(K, 2, sizeof(ae2f_float_t), uf.F);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;
    #endif

    if((ae2fCL_Ann.LErr = clEnqueueNDRangeKernel(
        ae2fCL_Ann.Q, K, 1,
        0, &_this->inc, &_this->inc, 
        0, 0, event
    )) != CL_SUCCESS) return (ae2f_errGlob_NFOUND);

    if((ae2fCL_Ann.LErr = clEnqueueReadBuffer(
        ae2fCL_Ann.Q, _W, 
        CL_TRUE, 0, 
        _this->inc * sizeof(ae2f_float_t), 
        ae2f_mAnnSpW(_this), 
        1, event, event + 1
    )) != CL_SUCCESS) return (ae2f_errGlob_NFOUND);
    ae2fCL_Ann.LErr = clReleaseEvent(event[0]); event[0] = 0;
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return(ae2f_errGlob_NFOUND);

    END:
    #undef return
    if(event[0]) {
        ae2fCL_Ann.LErr = clWaitForEvents(1, event);
        if(!ae2fCL_Ann.LErr)
            ae2fCL_Ann.LErr = clReleaseEvent(event[0]);
    }

    if(ae2fCL_Ann.LErr) {
        return ae2f_errGlob_FLUSH_FAILED | ae2f_errGlob_NFOUND;
    }

    if(event[1]) {
        ae2fCL_Ann.LErr = clWaitForEvents(1, event + 1);
        if(!ae2fCL_Ann.LErr)
            ae2fCL_Ann.LErr = clReleaseEvent(event[1]);
    }

    if(ae2fCL_Ann.LErr) {
        return ae2f_errGlob_FLUSH_FAILED | ae2f_errGlob_NFOUND;
    }

    return er;
}

ae2f_SHAREDEXPORT
size_t ae2fCL_mAnnSpInit(
    ae2fCL_mAnnSp* perc_opt,
    size_t icount,
    ae2f_float_t* Field_opt,
    ae2f_AnnAct_t vAct,
    ae2f_AnnAct_t vActDeriv,
    ae2f_AnnLoss_t vLossDeriv,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept {
    #define return(n) { err = n; goto END; }
    ae2f_err_t err = 0;
    cl_int err2 = 0;

    ae2f_mAnnSpInit(
        perc_opt
        , icount
        , Field_opt
        , vAct
        , vActDeriv
        , vLossDeriv
        , &err
        , off_opt
    );

    if(err) goto END;

    if(!Field_opt) {
        perc_opt->pField = ae2f_reinterpret_cast(ae2f_float_t*, ae2fCL_mAnnSpIOCl(perc_opt) + 1);
    }

    *ae2fCL_mAnnSpWCl(perc_opt) = clCreateBuffer(
        ae2fCL_Ann.Ctx, 
        CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
        ae2f_float_t_SIZE * icount, ae2f_mAnnSpW(perc_opt),
        &err2
    );
    if(err2 != CL_SUCCESS) 
    return(ae2f_errGlob_NFOUND | ae2f_errGlob_ALLOC_FAILED);

    *ae2fCL_mAnnSpIOCl(perc_opt) = clCreateBuffer(
        ae2fCL_Ann.Ctx,
        CL_MEM_READ_WRITE,
        ae2f_float_t_SIZE * (icount + 1), 0, &err2
    );
    if(err2 != CL_SUCCESS) 
    return(ae2f_errGlob_NFOUND | ae2f_errGlob_ALLOC_FAILED);

    perc_opt->vClean = Clean;
    perc_opt->vPredict = Predict;
    perc_opt->vTrain = Train;

    END:
    #undef return
    if(errret_opt) *errret_opt = err;
    if(erronnfound_opt) *erronnfound_opt = err2;
    return ae2fCL_mAnnSpInitSz(off_opt, icount);
}

ae2f_SHAREDEXPORT
ae2fCL_AnnSp* ae2fCL_AnnSpMk(
    size_t icount,
    ae2f_float_t* Field_opt,
    ae2f_AnnAct_t vAct,
    ae2f_AnnAct_t vActDeriv,
    ae2f_AnnLoss_t vLossDeriv,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept {
    ae2fCL_AnnSp* v;
    v = calloc(ae2fCL_mAnnSpInitSz(off_opt - (Field_opt ? (icount + 1) * sizeof(ae2f_float_t) : 0), icount), 1);
    ae2fCL_mAnnSpInit(
        &v->CL_Sp
        , icount
        , Field_opt
        , vAct
        , vActDeriv
        , vLossDeriv
        , errret_opt
        , erronnfound_opt
        , off_opt
    );
    return v;
}

static ae2f_err_t Clean(ae2f_mAnnSp* a) {
    if(!a) return ae2f_errGlob_PTR_IS_NULL;
    if(ae2fCL_Ann.LErr) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clReleaseMemObject(*ae2fCL_mAnnSpIOCl(a));
    if(ae2fCL_Ann.LErr) return ae2f_errGlob_NFOUND | ae2f_errGlob_FLUSH_FAILED;
    ae2fCL_Ann.LErr = clReleaseMemObject(*ae2fCL_mAnnSpWCl(a));
    if(ae2fCL_Ann.LErr) return ae2f_errGlob_NFOUND | ae2f_errGlob_FLUSH_FAILED;

    return ae2f_errGlob_OK;
}
