#include <ae2f/Ann/Slp.h>
#include <stdlib.h>
#include <stdio.h>

static ae2f_AnnSlpPredict_t Predict;
static ae2f_AnnSlpTrain_t Train;
static ae2f_AnnSlpClean_t Clean;

static ae2f_err_t Predict(
    const ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!out_opt) ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t err = 0;

    for(size_t i = 0; i < _this->outc; i++) {
        union {
            const size_t* pad;
            const ae2f_AnnSp* perc;
        }  layer = {ae2f_AnnSlpPerVPad(_this, const)[i]};

        size_t _pad = *layer.pad;
        layer.pad++;

        ae2f_err_t er = 
        ae2f_AnnSpPredict(
            layer.perc,
            in + _pad,
            out_opt + i
        );

        err |= er;
    }

    return err;
}

static ae2f_err_t Train(
    ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(learningrate == 0) return ae2f_errGlob_OK;

    ae2f_err_t _er = 0;

    #if 1
    for(size_t i = 0; i < _this->outc; i++) {
        ae2f_float_t G = goal_optB ? goal_optB[i] : 0;
        union {
            size_t* pad;
            ae2f_AnnSp* perc;
        }  layer = {ae2f_AnnSlpPerVPad(_this)[i]};

        size_t _pad = *layer.pad;
        layer.pad++;

        _er |= ae2f_AnnSpTrain(
            layer.perc, 
            in + _pad, 
            delta_optA, G, 
            learningrate
        );

        ae2f_float_t d;

        _er |= ae2f_AnnSpPredict(
            layer.perc,
            in + _pad,
            &d
        );
    }
    #else
    size_t i = 0;
    ae2f_float_t G = goal_optB ? goal_optB[i] : 0;
    union {
        size_t* pad;
        ae2f_AnnSp* perc;
    }  layer = {ae2f_AnnSlpPerVPad(_this)[i]};

    size_t _pad = *layer.pad;
    layer.pad++;

    _er |= ae2f_AnnSpTrain(
        layer.perc, 
        in + _pad, 
        delta_optA, G, 
        learningrate
    );
    #endif

    return _er;
}

static ae2f_err_t Clean(
    ae2f_AnnSlp* _this
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->layerc; i++) {
        const union {
            ae2f_AnnSp* v;
            size_t* S;
        } per = { .v = ae2f_AnnSlpPerV(_this, i)};
        ae2f_AnnSpClean(per.v);
        free(per.S - 1);
        
        ae2f_AnnSlpPerVPad(_this, )[i] = 0;
    }

    _this->inc = 0;
    _this->outc = 0;
    _this->layerc = 0;
    _this->vClean = 0;
    _this->vPredict = 0;
    _this->vTrain = 0;

    return ae2f_errGlob_OK;
}