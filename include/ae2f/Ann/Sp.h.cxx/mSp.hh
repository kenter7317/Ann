/**
 * @file Sp.hh
 * @author ae2f
 * @brief 
 * @version 0.2
 * @date 2025-02-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../Sp.h"

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
    ae2f_float_t goal_optB,
    ae2f_float_t learningrate
) noexcept;

template<typename T>
constexprfun const T* ae2f_TMP X() const;
 
template<typename T>
constexprfun T* ae2f_TMP X();

static constexprfun size_t ae2f_TMP sz(size_t off, size_t inc);
inline ae2f_TMP ~ae2f_mAnnSp();

constexprfun const ae2f_float_t* ae2f_TMP B() const;
constexprfun ae2f_float_t* ae2f_TMP B();

constexprfun const ae2f_float_t* ae2f_TMP W() const;
constexprfun ae2f_float_t* ae2f_TMP W();


#undef ae2f_TMP