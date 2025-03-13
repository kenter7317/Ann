#include "../Slp.h"

#define ae2f_TMP

constexprfun const ae2fCL_mAnnSlpMemX* ae2f_TMP Add() const;
constexprfun ae2fCL_mAnnSlpMemX*  ae2f_TMP Add();

constexprfun const ae2f_float_t* ae2f_TMP OutCache() const;
constexprfun ae2f_float_t* ae2f_TMP OutCache();

template<typename T>
constexprfun const T* ae2f_TMP X() const;

template<typename T>
constexprfun T* ae2f_TMP X();

#undef ae2f_TMP

private:
ae2f_mAnnSlp _this[1];