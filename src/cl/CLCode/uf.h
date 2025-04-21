#ifndef uf_t_h
#define uf_t_h

#include <ae2f/Float.auto.h>
#include <ae2fCL/Loc.h>
#include "Size/ae2f_float_t.auto.h"
#include "Size/cl_mem.auto.h"

#if ae2f_float_t_SIZE == 8
typedef uint32_t ae2f_float_half_t;
#elif ae2f_float_t_SIZE == 4
typedef uint16_t ae2f_float_half_t;
#else
#error ae2f_float_half_t is not here.
#endif

typedef union UF_t {
    ae2f_float_half_t UHalf[2];
    ae2f_float_t F[1];
} UF_t;

#endif
