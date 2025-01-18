#ifndef ae2fCL_AnnAct_h
#define ae2fCL_AnnAct_h

#include <ae2f/Float.h>
#include <math.h>

#define ae2fCL_mAnnActEnumDef(name) ae2fCL_eAnnAct##name
#define ae2fCL_mAnnActFuncDef(name) ae2fCL_eAnnAct##name##_func
#define ae2fCL_mAnnActFuncName(enumname) enumname##_func

typedef enum ae2fCL_eAnnAct {
    ae2fCL_eAnnAct_NULL,
    ae2fCL_mAnnActEnumDef(Sigmoid),
    ae2fCL_mAnnActEnumDef(Step),
    #include "Act/Extension.Enum.h"
    ae2fCL_eAnnAct_LEN
} ae2fCL_efAnnAct_t;

typedef ae2f_float_t(ae2fCL_fAnnAct_t)(ae2f_float_t x);

inline static ae2f_float_t 
ae2fCL_mAnnActFuncDef(Sigmoid)(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}
inline static ae2f_float_t 
ae2fCL_mAnnActFuncDef(Step)(ae2f_float_t x) {
    return x >= 0;
}

#include "Act/Extension.Function.h"
#endif