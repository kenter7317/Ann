#ifndef ae2fCL_AnnAct_h
#define ae2fCL_AnnAct_h

#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>


typedef ae2f_float_t(ae2fCL_fAnnAct_t)(ae2f_float_t x);
typedef ae2fCL_whenCL(void) ae2fCL_whenC(ae2fCL_fAnnAct_t)* ae2fCL_fpAnnAct_t;

#endif