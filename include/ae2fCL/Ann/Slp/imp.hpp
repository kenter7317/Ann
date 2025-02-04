/**
 * @file imp.hpp
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2025
 * 
 * 
 */

#ifndef ae2fCL_Ann_Slp_imp_hpp
#define ae2fCL_Ann_Slp_imp_hpp

#include "../Slp.h"

#pragma region SlpEl
#define ae2f_TMP ae2fCL_AnnSlpEl::

constexprfun size_t ae2f_TMP InLengthReq() {
    return ae2fCL_AnnSlpElInLengthReq(this);
}

#undef ae2f_TMP
#pragma endregion

#pragma region Slp
#define ae2f_TMP ae2fCL_AnnSlp::


#undef ae2f_TMP
#pragma endregion

#endif