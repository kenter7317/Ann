/**
 * @file Slp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * Slp definition
 * 
 */

#ifndef ae2fCL_Ann_Slp_h
#define ae2fCL_Ann_Slp_h

#include <ae2f/Ann/Slp.h>
#include <CL/cl.h>

ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_AnnSlpInit(
    ae2f_AnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept;

#define ae2fCL_AnnSlpInitA(_this, incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpInit(_this, incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

#define ae2fCL_AnnSlpInitB(_this, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpInit(_this, 0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)


#if ae2f_WhenCXX(!) 0 
struct ae2fCL_AnnSlp : ae2f_AnnSlp {
#include "Slp.h.cxx/Slp.hh" 
}
#else

/**
 * @class ae2fCL_AnnSlp
 * @brief 
 * SLP with OpenCL acceleration.
 */
typedef ae2f_AnnSlp ae2fCL_AnnSlp;
#endif

ae2f_extern ae2f_SHAREDCALL
ae2f_AnnSlp* ae2fCL_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept;

#define ae2fCL_AnnSlpMkA(incs, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpMk(incs, 0, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

#define ae2fCL_AnnSlpMkB(ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt) \
ae2fCL_AnnSlpMk(0, ginc, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err_opt, err_nfound_opt)

typedef struct ae2fCL_AnnSlpMemX {
	cl_mem In;
} ae2fCL_AnnSlpMemX;

/** 
 * @param outc
 * @param off 
 * */
#define ae2fCL_AnnSlpInitSz(outc, off) \
	ae2f_AnnSlpInitSz(outc, (off) + sizeof(ae2fCL_AnnSlpMemX))


#define ae2fCL_AnnSlpAdd(slp, ...) \
	ae2f_AnnSlpX(slp, ae2fCL_AnnSlpMemX*, __VA_ARGS__)

#define ae2fCL_AnnSlpX(slp, type, ...) \
 	ae2f_reinterpret_cast(__VA_ARGS__ type*, ae2fCL_AnnSlpAdd(slp, __VA_ARGS__) + 1)

#endif
