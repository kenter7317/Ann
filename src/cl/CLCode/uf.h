#ifndef uf_t_h
#define uf_t_h

#if cl_mem_SIZE == 8
typedef uint32_t cl_mem_half_t;
#endif

#if cl_mem_SIZE == 4
typedef uint16_t cl_mem_half_t;
#endif

typedef union UF_t {
    cl_mem_half_t UHalf[2];
    ae2f_float_t F[1];
} UF_t;


#endif