#include <ae2fCL/Ann/Mlp.h>
#include <ae2fCL/Ann/Slp.h>
#include <ann-h/Mlp.h>

ae2f_SHAREDEXPORT
size_t ae2fCL_AnnMlpInit(
    ae2f_mAnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt_unused,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt,
    cl_int* errnfound_opt
) noexcept {
    cl_int err2 = 0;
    ae2f_err_t err = 0;
    #define return(code) { err = code; goto EXIT; } 
    if(!_this) return(ae2f_errGlob_PTR_IS_NULL);
    if(!layerlenv) return(ae2f_errGlob_PTR_IS_NULL);
    if(layerpadv_opt_unused) err |= ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_IMP_NOT_FOUND;
    if(layerc == 1) return(ae2f_errGlob_WRONG_OPERATION);

    _this->expected = 1;
    size_t max = _this->inc = *layerlenv;
    _this->outc = layerlenv[layerc - 1];
    _this->vClean = Clean;
    _this->vPredict = Predict;
    _this->vTrain = Train;
    _this->layerc = --layerc;

    for(size_t i = 0; i < layerc; i++) {
        cl_int er_cl;

        size_t 
        LAYERSZ_L = layerlenv[i],
        LAYERSZ_R = layerlenv[i + 1];
        
        if(max < LAYERSZ_R)
        max = LAYERSZ_R;

        ae2f_err_t e = 0;

        union {
            size_t** unused;

            union {
                size_t* pad;
                ae2f_AnnSlp* slp;
            }* u;
        } perc = {ae2f_mAnnMlpLayerVPad(_this) + i};

        perc.u->pad = calloc(ae2fCL_AnnSlpInitSz(LAYERSZ_R, sizeof(size_t)), 1);
        perc.u->pad++;
        ae2fCL_AnnSlpInitB(perc.u->slp, LAYERSZ_L, 0, weights_opt, actglob_opt, deltaglob_opt, LAYERSZ_R, 0, &e, &er_cl);
        if(er_cl) err2 = er_cl;

        *(--perc.u->pad) = 0;
        if(weights_opt) weights_opt += LAYERSZ_L * LAYERSZ_R; 

        if(e)
        err = err | e & ~ae2f_errGlob_DONE_HOWEV;
    }

    ae2f_mAnnMlpCache(_this,)[0] = calloc(
        (max * layerc) << 2,
        sizeof(ae2f_float_t)
    );
    *ae2f_mAnnMlpLayerBuffCount(_this) = max;

    EXIT:
    #undef return
    if(errret_opt) {
        *errret_opt = err | (err2 ? ae2f_errGlob_NFOUND : 0);
    }
    if(errnfound_opt) *errnfound_opt= err2;
    return ae2f_mAnnMlpInitSz(++layerc, add_opt);
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
    ae2f_AnnMlp* obj = calloc(ae2f_mAnnMlpInitSz(layerc, add_opt), 1);

    ae2fCL_AnnMlpInit(
        &obj->Mlp, layerc, add_opt, 
        layerlenv, layerpadv_opt, 
        inpadv_opt, actglob_opt, 
        deltaglob_opt, weights_opt, 
        errret_opt, errnfound_opt
    );

    if(errret_opt) *errret_opt = err;
    if(errnfound_opt) *errnfound_opt = err2;
    return obj;
}