#include "../Slp.h"

#define ae2f_TMP

constexprfun const ae2fCL_mAnnSlpMemX* ae2f_TMP Add() const;
constexprfun ae2fCL_mAnnSlpMemX*  ae2f_TMP Add();

constexprfun const ae2f_float_t* ae2f_TMP OutCache() const;
constexprfun ae2f_float_t* ae2f_TMP OutCache();

constexprfun const cl_event* ae2f_TMP EventVec() const;
constexprfun cl_event* ae2f_TMP EventVec();
#undef ae2f_TMP

private:
ae2f_mAnnSlp _this[1];