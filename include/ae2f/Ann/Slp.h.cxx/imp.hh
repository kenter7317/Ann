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

#pragma region ae2f_AnnSlp
#define ae2f_TMP ae2f_AnnSlp::

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) const noexcept {
    return ae2f_AnnSlpPredict(this, in, out_opt);
}
 
inline ae2f_err_t ae2f_TMP Train(
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_AnnSlpTrainA(this, in, delta_optA, learningrate);
}

inline ae2f_err_t ae2f_TMP Train(
    const ae2f_float_t* in,
    ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    return ae2f_AnnSlpTrainB(this, in, goal_optB, learningrate);
}

inline ae2f_TMP ~ae2f_AnnSlp() {
    ae2f_AnnSlpClean(this);
}

template<typename T>
constexprfun const T* ae2f_TMP X() const {
    return ae2f_AnnSlpX(this, T*, const);
}
 
template<typename T>
constexprfun T* ae2f_TMP X() {
    return ae2f_AnnSlpX(this, T*);
}

constexprfun ae2f_AnnSlpEl* ae2f_TMP Perc(size_t i) {
    return ae2f_AnnSlpPerV(this, i);
}
constexprfun const ae2f_AnnSlpEl* ae2f_TMP Perc(size_t i) const {
    return ae2f_AnnSlpPerV(this, i, const);
}

#undef ae2f_TMP
#pragma endregion