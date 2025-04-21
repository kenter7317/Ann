/**
 * @file Slp.hh
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../Slp.h"

#define ae2f_TMP

inline ae2f_err_t ae2f_TMP Predict(
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) const noexcept;
 
inline ae2f_err_t ae2f_TMP TrainA(
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    ae2f_float_t learningrate
) noexcept;

inline ae2f_err_t ae2f_TMP TrainB(
    const ae2f_float_t* in,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept;

constexprfun ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t);
constexprfun const ae2f_mAnnSlpEl* ae2f_TMP Perc(size_t) const;

constexprfun size_t* ae2f_TMP PercPad(size_t);
constexprfun const size_t* ae2f_TMP PercPad(size_t) const;

constexprfun const ae2f_float_t* ae2f_TMP Field() const;
constexprfun ae2f_float_t* ae2f_TMP Field();

constexprfun const ae2f_float_t* ae2f_TMP OutCache() const;
constexprfun ae2f_float_t* ae2f_TMP OutCache();

#undef ae2f_TMP