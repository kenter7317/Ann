#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Sp.h>
#include "CLCode/Size/ae2f_float_t.auto.h"
#include "CLCode/Size/cl_mem.auto.h"

static ae2f_mAnnSpPredict_t Predict;
static ae2f_mAnnSpTrain_t Train;
static ae2f_mAnnSpClean_t Clean;

static ae2f_err_t Predict(
    const ae2f_mAnnSp *_this, 
    const ae2f_float_t *in, 
    ae2f_float_t *outret_opt
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!outret_opt) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;

    cl_mem _W = *ae2fCL_mAnnSpWCl(_this, const), _IO = *ae2fCL_mAnnSpIOCl(_this, const);
    cl_kernel K = ae2fCL_Ann.Kerns[ae2fCL_eAnnKernsSpPredict];
    ae2f_float_t IBuffer;

    ae2fCL_Ann.LErr = clEnqueueWriteBuffer(ae2fCL_Ann.Q, _IO, CL_TRUE, 0, ae2f_float_t_SIZE * _this->inc, in, 0, 0, 0);
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
        0, 0, 0
    );
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    ae2fCL_Ann.LErr = clEnqueueReadBuffer(ae2fCL_Ann.Q, _IO, CL_TRUE, _this->inc * ae2f_float_t_SIZE, ae2f_float_t_SIZE, &IBuffer, 0, 0, 0);
    if(ae2fCL_Ann.LErr != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    IBuffer = _this->Act(IBuffer + *ae2f_mAnnSpB(_this, const));
    if(outret_opt) *outret_opt = IBuffer;

    return ae2f_errGlob_OK;
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
    ae2f_err_t er = 0;

    UF_t uf;
    if(delta_optA) 
        *uf.F = *delta_optA;
    else {
        er = Predict(_this, in, uf.F);
        *uf.F = _this->CalDelta(*uf.F, goal_optB);
    }
    if(er) return er;

    *uf.F *= learningrate;
    *ae2f_mAnnSpB(_this) += *uf.F;

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
        0, 0, 0
    )) != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    if((ae2fCL_Ann.LErr = clEnqueueReadBuffer(
        ae2fCL_Ann.Q, _W, 
        CL_TRUE, 0, 
        _this->inc * sizeof(ae2f_float_t), 
        ae2f_mAnnSpW(_this), 
        0, 0, 0
    )) != CL_SUCCESS) return ae2f_errGlob_NFOUND;

    return er;
}

ae2f_SHAREDEXPORT
size_t ae2fCL_mAnnSpInit(
    ae2fCL_mAnnSp* perc_opt,
    size_t icount,
    const ae2f_float_t* w_opt,
    ae2f_AnnAct_t Act,
    ae2f_AnnDelta_t CalDelta,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept {
    #define return(n) { err = n; goto END; }
    ae2f_err_t err = 0;
    cl_int err2 = 0;

    ae2f_mAnnSpInit(perc_opt, icount, w_opt, Act, CalDelta, &err, off_opt);
    if(err) goto END;

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
    const ae2f_float_t* w_opt,
    ae2f_AnnAct_t Act,
    ae2f_AnnDelta_t CalDelta,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept {
    ae2fCL_AnnSp* v;
    v = calloc(ae2fCL_mAnnSpInitSz(off_opt, icount), 1);
    ae2fCL_mAnnSpInit(&v->CL_Sp, icount, w_opt, Act, CalDelta, errret_opt, erronnfound_opt, off_opt);
    return v;
}

static ae2f_err_t Clean(ae2f_mAnnSp* a) {
    if(!a) return ae2f_errGlob_PTR_IS_NULL;
    cl_int er[2];
    er[0] = clReleaseMemObject(*ae2fCL_mAnnSpIOCl(a));
    er[1] = clReleaseMemObject(*ae2fCL_mAnnSpWCl(a));

    if(er[0] || er[1])
    return ae2f_errGlob_NFOUND;

    return ae2f_errGlob_OK;
}
