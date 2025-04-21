/**
 * @file Mlp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <ae2f/Ann/Mlp.h>

#include <memory.h>
#include "Mlp/HidErr.h"
#include "Mlp/HidCompute.h"
#include "Mlp/OutCompute.h"
#include "Mlp/Predict.h"

static ae2f_mAnnMlpPredict_t Predict;
static ae2f_mAnnMlpTrain_t Train;
static ae2f_mAnnMlpClean_t Clean;

static ae2f_err_t Predict (
    const ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outret_opt
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(!outret_opt) return ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL;

    size_t A = *ae2f_mAnnMlpLayerBuffCount(_this, const);

    union {
        const ae2f_float_t* CF;
        ae2f_float_t* F;
        uintptr_t P;
    }
    tmpVi = { in }, 
    tmpVo = { *ae2f_mAnnMlpCache(_this, const) };

    if(!tmpVo.P) return ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t err = 0;

    for(size_t i = 0; i < _this->layerc; i++) {
        const ae2f_mAnnSlp* slp = ae2f_mAnnMlpLayerV(_this, i, const);
        if(i == _this->layerc - 1)
        tmpVo.F = outret_opt;

        err |= ae2f_mAnnSlpPredict(
            slp,
            tmpVi.CF, tmpVo.F
        );

        tmpVi.F = tmpVo.F;  tmpVo.F += A;
    }

    return err;
}

static ae2f_err_t Train (
    ae2f_mAnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    ae2f_err_t ret = 0;
    #define return(n) {  ret |= n; goto RET; }
    const size_t
    MAXBUFFCOUNT_FOR_LAYER = *ae2f_mAnnMlpLayerBuffCount(_this),
    SkipInput = MAXBUFFCOUNT_FOR_LAYER * _this->layerc;

    #define _cache_ALLOCCOUNT SkipInput * 3
    #define _cache_SIZE sizeof(ae2f_float_t)

    ae2f_float_t* const _cache = *ae2f_mAnnMlpCache(_this);
    #define _cache_Out _cache

    if(!_cache) return(ae2f_errGlob_ALLOC_FAILED);
    ae2f_float_t* const cache_Deltas = _cache + SkipInput;
    ae2f_float_t* const cache_Out2 = cache_Deltas + SkipInput;

    ret |= MlpTrain_Predict(
        _this, in, _cache_Out
    ); if(ret) goto RET;
    
    memcpy(cache_Out2, _cache_Out, sizeof(ae2f_float_t) * SkipInput);

    for(size_t i = _this->layerc - 1; i != ((size_t)-1); i--) {
        ae2f_mAnnSlp
        * const LAYER = ae2f_mAnnMlpLayerV(_this, i),
        * const LAYERNXT = ae2f_mAnnMlpLayerV(_this, i + 1);

        if(i == _this->layerc - 1) {
            if (delta_optA)
            memcpy(
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER, 
                delta_optA, 
                _this->outc * sizeof(ae2f_float_t)
            );
            
            else if(goal_optB) 
            MlpTrain_OutCompute(
                LAYER,
                goal_optB, 
                _cache_Out + i * MAXBUFFCOUNT_FOR_LAYER,
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER
            ); 
            else return(ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_WRONG_OPERATION);
        } else {
            MlpTrain_HidCompute(
                LAYER,
                LAYERNXT,
                cache_Deltas + i * MAXBUFFCOUNT_FOR_LAYER,
                cache_Deltas + (i + 1) * MAXBUFFCOUNT_FOR_LAYER,
                _cache_Out + (i) * MAXBUFFCOUNT_FOR_LAYER
            );
        }

        puts("asdf1");

        printf("in %llu out %llu\n", LAYER->inc, LAYER->outc);
        printf("%f %f\n", in[0], in[1]);
        printf("%llu\n", MAXBUFFCOUNT_FOR_LAYER);
        
        ret |= ae2f_mAnnSlpTrainA(
            LAYER,
            i ? cache_Out2 + MAXBUFFCOUNT_FOR_LAYER * (i - 1) : in,  
            cache_Deltas + (i) * MAXBUFFCOUNT_FOR_LAYER,
            learningrate
        );

        puts("asdf2");
    }

    RET:
    #undef return
    #undef _cache_Out
    return ret;
}

static ae2f_err_t Clean(
    ae2f_mAnnMlp* _this
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->layerc; i++) {
        union {
            size_t** unused;

            union {
                size_t* pad;
                ae2f_mAnnSlp* slp;
            }* u;
        } perc = { .unused = ae2f_mAnnMlpLayerVPad(_this) + i };
        if(!perc.u->pad) break;
        perc.u->pad++;
        ae2f_mAnnSlpClean(perc.u->slp);
        free(perc.u->pad - 1);
    }

    if(*ae2f_mAnnMlpCache(_this)) 
    free(*ae2f_mAnnMlpCache(_this));

    _this->inc = 0;
    _this->outc = 0;
    _this->vClean = 0;
    _this->vPredict = 0;
    _this->vTrain = 0;

    return ae2f_errGlob_OK;
}
