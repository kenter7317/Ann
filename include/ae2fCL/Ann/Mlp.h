#ifndef ae2fCL_Ann_Mlp_h
#define ae2fCL_Ann_Mlp_h

#include <ae2f/Ann/Mlp.h>
#include <CL/cl.h>

ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_AnnMlpInit(
    ae2f_AnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt,
    cl_int* errnfound_opt
) noexcept;

ae2f_extern ae2f_SHAREDCALL
ae2f_AnnMlp* ae2fCL_AnnMlpMk(
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt,
    cl_int* errnfound_opt
) noexcept;

#define ae2fCL_AnnMlpInitSz ae2f_AnnMlpInitSz
#define ae2fCL_AnnMlpDel    ae2f_AnnMlpDel
#define ae2fCL_AnnMlpClean  ae2f_AnnMlpClean

#endif