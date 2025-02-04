/**
 * @file Slp.hpp
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2fCL_Ann_Slp_Slp_hpp
#define ae2fCL_Ann_Slp_Slp_hpp
#define ae2f_TMP

#include "../Slp.h"

constexprfun const ae2fCL_fpAnnAct_t& ae2f_TMP GetAct(size_t i) const noexcept;
constexprfun ae2fCL_fpAnnAct_t& ae2f_TMP GetAct(size_t i) noexcept;

constexprfun const ae2fCL_fpAnnSpGetLoss_t& ae2f_TMP GetLoss(size_t i) const noexcept;
constexprfun ae2fCL_fpAnnSpGetLoss_t& ae2f_TMP GetLoss(size_t i) noexcept;

#undef ae2f_TMP
#endif