#include <ae2fCL/Ann/Mlp.h>
#include <ae2fCL/Ann/Slp.h>
#include <ann-h/Mlp.h>

ae2f_SHAREDEXPORT
size_t ae2fCL_AnnMlpInit(
    ae2f_AnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt,
    cl_int* errnfound_opt
) noexcept {
    ae2f_err_t err = 0;
    cl_int err2 = 0, _err2 = 0;
    #define return(n) { err |= (n); goto RET; }
    if(!_this) return (ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV);
    if(!layerlenv) return (ae2f_errGlob_PTR_IS_NULL);
    if(!layerc) return (ae2f_errGlob_WRONG_OPERATION);
    
    size_t MAX = 
    _this->inc = 
    *layerlenv + (layerpadv_opt ? *layerpadv_opt : 0),
    
    OUT_SIZE = 0;

    _this->expected = 1;
    _this->vPredict = Predict;
    _this->vTrain = Train;
    _this->vClean = Clean;
    _this->layerc = layerc;
    
    for(size_t i = 0; i < layerc - 1; i++) {
        ae2f_err_t _err;

        size_t
        ** const _LAYER = ae2f_AnnMlpLayerVPad(_this, ) + i,
        * const LAYER = *_LAYER = calloc(ae2f_AnnSlpInitSz(layerlenv[i - 1], sizeof(size_t)), 1),
        LAYERIN_C = layerlenv[i];

        OUT_SIZE = layerlenv[i + 1];

        if(layerpadv_opt) {
            *LAYER = layerpadv_opt[i];
            OUT_SIZE += layerpadv_opt[i + 1];
        }

        if(MAX < OUT_SIZE) MAX = OUT_SIZE;

        ae2fCL_AnnSlpInitB(
            (void*)(LAYER + 1), 
            layerlenv[i], 
            inpadv_opt, weights_opt, actglob_opt,
            deltaglob_opt, layerlenv[i + 1],
            add_opt, &_err, &_err2
        );

        err |= _err;
        if(err & ae2f_errGlob_NFOUND)
        err2 = _err2;

        if(weights_opt)
        weights_opt += LAYERIN_C;
        
        if(inpadv_opt)
        inpadv_opt += LAYERIN_C;
    }

    _this->outc = OUT_SIZE;
    *ae2f_AnnMlpLayerBuffCount(_this) = MAX;
    if(!(*ae2f_AnnMlpCache(_this) = calloc(MAX * _this->layerc * 3, sizeof(ae2f_float_t))))
    return(ae2f_errGlob_ALLOC_FAILED);

    RET:
    #undef return
    if(errret_opt) *errret_opt = err;
    if(errnfound_opt) *errnfound_opt = err2;
    return ae2f_AnnMlpInitSz(layerc, add_opt);
}

ae2f_extern ae2f_SHAREDCALL
ae2f_AnnMlp* ae2fCL_AnnMlpMk(
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt,
    cl_int* errnfound_opt
) noexcept {
    ae2f_err_t err = 0;
    cl_int err2 = 0;
    ae2f_AnnMlp* obj = calloc(ae2f_AnnMlpInitSz(layerc, add_opt), 1);

    ae2fCL_AnnMlpInit(
        obj, layerc, add_opt, 
        layerlenv, layerpadv_opt, 
        inpadv_opt, actglob_opt, 
        deltaglob_opt, weights_opt, 
        errret_opt, errnfound_opt
    );

    if(errret_opt) *errret_opt = err;
    if(errnfound_opt) *errnfound_opt = err2;
    return obj;
}