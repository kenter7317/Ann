#ifndef uf_t_h
#define uf_t_h

#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>

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
