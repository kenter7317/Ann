/**
 * @file clean.script.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 * Deleting function macro
 */

#if ae2fCL_whenCL(!) 1

if(!_this) return ae2f_errGlob_PTR_IS_NULL;
ae2f_err_t er = 0;

if(_this->vClean)
er = _this->vClean(_this);

free(_this);
return er;

#else

return ae2f_errGlob_IMP_NOT_FOUND;

#endif