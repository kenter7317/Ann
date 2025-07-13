/**
 * @file cleandef.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2f_Ann_Sp_h_c_cleandef_h
#define ae2f_Ann_Sp_h_c_cleandef_h

#include <ae2f/errGlob.h>
#include <stdlib.h>

#define ae2f_AnnDelDef(classname) \
ae2f_err_t inline static classname##Del(classname* _this)

#endif