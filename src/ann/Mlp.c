#include <ae2f/Ann/Mlp.h>

#include <memory.h>
#include "Mlp/HidErr.h"
#include "Mlp/HidCompute.h"
#include "Mlp/OutCompute.h"
#include "Mlp/Predict.h"

static ae2f_AnnMlpPredict_t Predict;
static ae2f_AnnMlpTrain_t Train;

static ae2f_err_t Predict (
    const ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outret_opt
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!outret_opt) return ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL;

    size_t A = *ae2f_AnnMlpLayerBuffCount(_this, const);

    union {
        const ae2f_float_t* CF;
        ae2f_float_t* F;
        uintptr_t P;
    }
    tmpVi = { in }, 
    tmpVo = { *ae2f_AnnMlpCache(_this) + A };

    if(!tmpVi.P) return ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t err = 0;

    for(size_t i = 0; i < _this->outc; i++) {
        err |= ae2f_AnnSlpPredict(
            ae2f_AnnMlpLayerV(_this, i, const),
            tmpVi.F, tmpVo.F
        );

        if(tmpVi.F == in) tmpVi.F = tmpVo.F - A;

        tmpVi.P ^= tmpVo.P;
        tmpVo.P ^= tmpVi.P;
        tmpVi.P ^= tmpVo.P;
    }

    memcpy(outret_opt, tmpVo.F, _this->outc);
    return err;
}

static ae2f_err_t Train (
    ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    ae2f_err_t ret = 0;
    #define return(n) { ret |= n; goto RET; }
    const size_t
    MAXBUFFCOUNT_FOR_LAYER = *ae2f_AnnMlpLayerBuffCount(_this),
    SkipInput = MAXBUFFCOUNT_FOR_LAYER * _this->layerc;

    #define _cache_ALLOCCOUNT SkipInput * 3
    #define _cache_SIZE sizeof(ae2f_float_t)

    ae2f_float_t* const _cache = *ae2f_AnnMlpCache(_this);
    if(!_cache) return(ae2f_errGlob_ALLOC_FAILED);
    ae2f_float_t* const cache_Deltas = _cache + SkipInput;
    ae2f_float_t* const cache_Goals = cache_Deltas + SkipInput;

    ret |= MlpTrain_Predict(
        _this, in, _cache
    ); if(ret) goto RET;

    for(size_t i = _this->layerc - 1; i != ((size_t)-1); i--) {
        ae2f_AnnSlp
        * const LAYER = ae2f_AnnMlpLayerV(_this, i),
        * const LAYERNXT = ae2f_AnnMlpLayerV(_this, i + 1);

        if(i == _this->layerc - 1) {
            MlpTrain_OutCompute(
                LAYER,
                goal_optB, 
                _cache + i * MAXBUFFCOUNT_FOR_LAYER,
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER
            );

            ret |= ae2f_AnnSlpTrainB(
                LAYER, 
                i ? _cache + MAXBUFFCOUNT_FOR_LAYER * (i - 1) : in,
                goal_optB, learningrate
            );

        } else {
            MlpTrain_HidCompute(
                LAYER,
                LAYERNXT,
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER,
                cache_Deltas + (i + 1) * MAXBUFFCOUNT_FOR_LAYER,
                _cache + (i) * MAXBUFFCOUNT_FOR_LAYER
            );

            ret |= ae2f_AnnSlpTrainA(
                LAYER,
                i ? _cache + MAXBUFFCOUNT_FOR_LAYER * (i - 1) : in,  
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER,
                learningrate
            );
        }
    }

    RET:
    #undef return
    if(_cache) free(_cache);
    return ret;
}

static ae2f_err_t Clean(
    ae2f_AnnSlp* _this
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->outc; i++) {
        ae2f_AnnSlpDel(ae2f_AnnMlpLayerV(_this, i));
        ae2f_AnnMlpLayerVPad(_this, )[i] = 0;
    }

    if(ae2f_AnnMlpCache(_this)) free(ae2f_AnnMlpCache(_this));

    _this->inc = 0;
    _this->outc = 0;
    _this->vClean = 0;
    _this->vPredict = 0;
    _this->vTrain = 0;

    return ae2f_errGlob_OK;
}

ae2f_SHAREDEXPORT
size_t ae2f_AnnMlpInit(
    ae2f_AnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) {
    ae2f_err_t err = 0;
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
            OUT_SIZE *= layerpadv_opt[i + 1];
        }

        if(MAX < OUT_SIZE) MAX = OUT_SIZE;

        ae2f_AnnSlpInitB(
            (void*)(LAYER + 1), 
            layerlenv[i], 
            inpadv_opt, weights_opt, actglob_opt,
            deltaglob_opt, layerlenv[i + 1],
            0, &_err
        );

        err |= _err;

        if(weights_opt)
        weights_opt += LAYERIN_C;
        
        if(inpadv_opt)
        inpadv_opt += LAYERIN_C;
    }

    _this->outc = OUT_SIZE;
    *ae2f_AnnMlpLayerBuffCount(_this) = MAX;
    if(!(*ae2f_AnnMlpCache(_this) = calloc(MAX * _this->layerc, 3 * sizeof(ae2f_float_t))))
    return(ae2f_errGlob_ALLOC_FAILED);

    RET:
    #undef return
    if(errret_opt) *errret_opt = err;
    return ae2f_AnnMlpInitSz(layerc, add_opt);
}