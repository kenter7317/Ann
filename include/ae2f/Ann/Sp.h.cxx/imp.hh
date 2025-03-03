/**
 * @file imp.hh
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../Sp.h"

#define ae2f_TMP ae2f_AnnSp::

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) const noexcept {
    return ae2f_AnnSpPredict(this, in, out_opt);
}

inline ae2f_err_t ae2f_TMP Train(
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_AnnSpTrainA(this, in, delta_optA, learningrate);
}

inline ae2f_err_t ae2f_TMP Train(
    const ae2f_float_t* in,
    ae2f_float_t goal_optB,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_AnnSpTrainB(this, in, goal_optB,  learningrate);
}

constexprfun const ae2f_float_t* ae2f_TMP B() const {
    return ae2f_AnnSpB(this, const);
}
constexprfun ae2f_float_t* ae2f_TMP B() {
    return ae2f_AnnSpB(this);
}

constexprfun const ae2f_float_t* ae2f_TMP W() const {
    return ae2f_AnnSpW(this, const);
}
constexprfun ae2f_float_t* ae2f_TMP W() {
    return ae2f_AnnSpW(this, );
}

template<typename T>
constexprfun const T* ae2f_TMP X() const {
    return ae2f_AnnSpX(this, T*, const);
}
 
template<typename T>
constexprfun T* ae2f_TMP X() {
    return ae2f_AnnSpX(this, T*, );
}

inline ae2f_TMP ~ae2f_AnnSp() {
    ae2f_AnnSpClean(this);
}

constexprfun size_t ae2f_TMP sz(size_t off, size_t inc) {
    return ae2f_AnnSpInitSz(off, inc);
}

#undef ae2f_TMP