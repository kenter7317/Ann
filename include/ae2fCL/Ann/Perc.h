/// @file Perceptron
#ifndef ae2fCL_Ann_Perc_h
#define ae2fCL_Ann_Perc_h

#include <ae2f/Cast.h>
#include <ae2f/Call.h>
#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2f/errGlob.h>

#define ae2fCL_fAnnPercAct_t uint8_t

#include <ae2f/Pack/Beg.h>
typedef struct ae2fCL_AnnPerc {
      ae2f_float_t m_bias;
      ae2fCL_HostPtr(__global, ae2f_float_t) mg_field;
      size_t mg_fieldLen;
      ae2fCL_fAnnPercAct_t act;
} ae2fCL_AnnPerc;
#include <ae2f/Pack/End.h>

ae2f_extern ae2f_SHAREDCALL ae2f_err_t ae2fCL_AnnPercMk(
    ae2f_struct ae2fCL_AnnPerc* _this,
    const ae2f_float_t* inputs,
    size_t inputsCount,
    ae2fCL_fAnnPercAct_t act,
    cl_context ctx,
    cl_command_queue queue,
    cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list,
    cl_event *event
);

ae2f_extern ae2f_SHAREDCALL
ae2f_err_t ae2fCL_AnnPercDel(
    ae2fCL_AnnPerc* _this
);

#define ae2fCL_AnnPercCheck(_this, _out, queue) \ 
clEnqueueReadBuffer( \
      queue,  \
      (_this)->mg_field, \
      CL_TRUE, 0, \
      sizeof(ae2f_float_t) * (_this)->mg_fieldLen, \
      (_out), \
      0, 0, 0 \
)

#endif