#include "../Slp.h"

#define ae2f_TMP ae2fCL_mAnnSlp::

constexprfun const ae2fCL_mAnnSlpMemX* ae2f_TMP Add() const {
    return ae2fCL_mAnnSlpAdd(_this, const);
}
constexprfun ae2fCL_mAnnSlpMemX*  ae2f_TMP Add() {
    return ae2fCL_mAnnSlpAdd(_this);
}

#undef ae2f_TMP

#define  ae2f_TMP ae2fCL_AnnSlp::

inline ae2f_TMP ~ae2fCL_AnnSlp() {
    ae2f_AnnSlpClean(this);
}
#undef ae2f_TMP