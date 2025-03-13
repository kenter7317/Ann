/**
 * @file Mlp.hh
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * Additional definition for @ref ae2f_mAnnMlp, @ref ae2f_mAnnMlpEl
 * 
 */

#include "../Mlp.h"

#define ae2f_TMP
constexprfun ae2f_mAnnMlpEl* ae2f_TMP Perc(size_t a);
constexprfun const ae2f_mAnnMlpEl* ae2f_TMP Perc(size_t a) const;

constexprfun size_t* ae2f_TMP PercPad(size_t);
constexprfun const size_t* ae2f_TMP PercPad(size_t) const;
#undef ae2f_TMP

private:
ae2f_mAnnSlp _this[1];