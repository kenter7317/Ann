/**
 * @file Sp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2025
 * 
 * Inline function for separated friends
 * This file is hidden.
 * 
 * @todo
 * Loggging functions are here. 
 * kill it.
 * 
 */
#include <ae2f/Ann/Sp.h>

#define ae2f_mAnnSpPredictI(_this, in, i, ...) (ae2f_mAnnSpW(_this, __VA_ARGS__ const)[i] * (in)[i])
#define ae2f_mAnnSpTrainI(_this, in, i, ...) (ae2f_mAnnSpW(_this, __VA_ARGS__)[i] -= _delta * (in)[i])

static ae2f_mAnnSpPredict_t Predict;
static ae2f_mAnnSpTrain_t Train;

static
ae2f_err_t Predict(
    const ae2f_mAnnSp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) {
    if(!in) return (ae2f_errGlob_PTR_IS_NULL);
    if(!_this) return (ae2f_errGlob_PTR_IS_NULL);
    if(!out_opt) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;    

    ae2f_float_t sum = 0;
    for(size_t i = 0; i < _this->inc; i++) {
        sum += ae2f_mAnnSpPredictI(_this, in, i);
    }

    sum += *ae2f_mAnnSpB(_this, const);

    if(_this->Act) sum = (_this)->Act(sum);

    __DONE:
    if(out_opt) *out_opt = sum;
    return ae2f_errGlob_OK;
}

static
ae2f_err_t Train(
    ae2f_mAnnSp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t goal_optB,
    ae2f_float_t learningrate
) {
    ae2f_err_t err = ae2f_errGlob_OK;
    ae2f_float_t _delta = 0;

    if(!_this) return (ae2f_errGlob_PTR_IS_NULL);
    if(!in) return (ae2f_errGlob_PTR_IS_NULL);
    if(learningrate == 0) return (ae2f_errGlob_OK);

    if(delta_optA) 
        _delta = *delta_optA;
    else {
        if(!_this->LossDeriv) return ae2f_errGlob_IMP_NOT_FOUND;
        err = ae2f_mAnnSpPredict(_this, in, &_delta);
        if(err) goto __DONE;
        _delta = (_this->ActDeriv ? 
            _this->ActDeriv(_delta) : _delta) * 
            _this->LossDeriv(&_delta, &goal_optB, 0, 1
            );
    }

    _delta *= learningrate;

    for(size_t i = 0; i < _this->inc; i++) {
        ae2f_mAnnSpW(_this, )[i] -= _delta * in[i];
    }
    
    *ae2f_mAnnSpB(_this) -= _delta;

    __DONE:
    return err;
}