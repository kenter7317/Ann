#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann/Sp.h>
#include <ann-h/Slp.h>

ae2f_SHAREDEXPORT
size_t ae2fCL_AnnSlpInit(
    ae2f_AnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* errnfound_opt
) noexcept {
    ae2f_err_t er = 0, ertmp = 0;
    cl_int V = 0, v2 = 0;
    #define return(c) { er = c; goto DONE; }

    if(!_this) return(ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_DONE_HOWEV);

    _this->expected = 1;
    _this->inc = 0;
    _this->outc = _this->layerc = outc;
    _this->vClean = Clean;
    _this->vPredict = Predict;
    _this->vTrain = Train;

    for(size_t i = 0; i < outc; i++) {
        size_t 
        _inc =  incs_optA ? incs_optA[i] : ginc_optB,
        _pad = inpads_opt ? inpads_opt[i] : 0;

        ae2f_AnnSlpPerVPad(_this)[i]
        = calloc(ae2fCL_AnnSpInitSz(sizeof(size_t), _inc), 1);

        ae2fCL_AnnSpInit(
            ae2f_AnnSlpPerV(_this, i),
            _inc, w_opt,
            Act, CalDelta,
            &ertmp, &v2, 0
        );

        if(v2 != CL_SUCCESS) V = v2;

        er |= ertmp;
        *ae2f_AnnSlpPerVPad(_this)[i] = _pad;

        w_opt && (w_opt += _inc);

        if(_this->inc < _pad + _inc) {
            _this->inc = _pad + _inc;
        }
    }

    #undef return
    DONE:
    if(err_opt) *err_opt = er;
    if(errnfound_opt) *errnfound_opt = V;
    return ae2f_AnnSlpInitSz(outc, offset_opt);
}

ae2f_SHAREDEXPORT
ae2f_AnnSlp* ae2fCL_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept {
    ae2f_AnnSlp* rtn = calloc(ae2f_AnnSlpInitSz(outc, offset_opt), 1);
    ae2f_err_t err = 0;
    cl_int err2 = 0;

    ae2fCL_AnnSlpInit(rtn, incs_optA, ginc_optB, inpads_opt, w_opt, Act, CalDelta, outc, 0, &err, &err2);

    if(err_opt) *err_opt = err;
    if(err_nfound_opt) *err_nfound_opt = err2;
    return rtn;
}