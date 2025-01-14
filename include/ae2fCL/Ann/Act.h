#ifndef ae2fCL_AnnAct_h
#define ae2fCL_AnnAct_h

#include <ae2f/Float.h>
#include <math.h>

typedef enum ae2fCL_eAnnAct {
    ae2fCL_eAnnAct_NULL,
    ae2fCL_eAnnActSigmoid,
    #include "Act/Extension.Enum.h"
    ae2fCL_eAnnAct_LEN
} ae2fCL_efAnnAct_t;

typedef ae2f_float_t(ae2fCL_fAnnAct_t)(ae2f_float_t x);

inline static ae2f_float_t __ae2fCL_eAnnActSigmoid(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

#include "Act/Extension.Function.h"
#endif