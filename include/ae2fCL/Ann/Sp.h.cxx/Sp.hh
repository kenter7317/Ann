#include "../Sp.h"

#define ae2f_TMP

constexprfun cl_mem* ae2f_TMP WCl();
constexprfun const cl_mem* ae2f_TMP WCl() const;

constexprfun cl_mem* ae2f_TMP IOCl();
constexprfun const cl_mem* ae2f_TMP IOCl() const;

template<typename T>
constexprfun const T* ae2f_TMP X() const;
 
template<typename T>
constexprfun T* ae2f_TMP X();

#undef ae2f_TMP