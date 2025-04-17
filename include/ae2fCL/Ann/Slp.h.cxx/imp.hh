#include "../Slp.h"

#define ae2f_TMP ae2fCL_mAnnSlp::

constexprfun const ae2fCL_mAnnSlpMemX* ae2f_TMP Add() const {
    return ae2fCL_mAnnSlpAdd(_this, const);
}
constexprfun ae2fCL_mAnnSlpMemX*  ae2f_TMP Add() {
    return ae2fCL_mAnnSlpAdd(_this);
}

constexprfun const ae2f_float_t* ae2f_TMP OutCache() const {
    return ae2fCL_mAnnSlpOutCache(_this, const);
}

constexprfun ae2f_float_t* ae2f_TMP OutCache() {
    return ae2fCL_mAnnSlpOutCache(_this);
}

constexprfun const cl_event* ae2f_TMP EventVec() const {
    return ae2fCL_mAnnSlpEventVec(_this, const);
}
constexprfun cl_event* ae2f_TMP EventVec() {
    return ae2fCL_mAnnSlpEventVec(_this);
}

template<typename T>
constexprfun const T* ae2f_TMP X() const {
    return ae2fCL_mAnnSlpX(_this, T, const);
}

template<typename T>
constexprfun T* ae2f_TMP X() {
    return ae2fCL_mAnnSlpX(_this, T, const);
}

#undef ae2f_TMP