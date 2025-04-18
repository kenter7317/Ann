/**
 * @file imp.hh
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../Slp.h"

#pragma region ae2f_mAnnSlp
#define ae2f_TMP ae2f_mAnnSlp::

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) const noexcept {
    return ae2f_mAnnSlpPredict(this, in, out_opt);
}
 
inline ae2f_err_t ae2f_TMP TrainA(
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_mAnnSlpTrainA(this, in, delta_optA, learningrate);
}

inline ae2f_err_t ae2f_TMP TrainB(
    const ae2f_float_t* in,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_mAnnSlpTrainB(this, in, goal_optB, learningrate);
}

constexprfun ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t i) {
    return ae2f_mAnnSlpPerV(this, i);
}
constexprfun const ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t i) const {
    return ae2f_mAnnSlpPerV(this, i, const);
}

constexprfun size_t* ae2f_TMP PercPad(size_t i) {
    return ae2f_mAnnSlpPerVPad(this)[i];
}
constexprfun const size_t* ae2f_TMP PercPad(size_t i) const {
    return ae2f_mAnnSlpPerVPad(this, const)[i];
}

#undef ae2f_TMP
#pragma endregion

#define ae2f_TMP ae2f_AnnSlp::
inline ae2f_TMP ~ae2f_AnnSlp() {
    ae2f_AnnSlpClean(this);
}
#undef ae2f_TMP