#ifndef mac_h
#define mac_h

#if !__ae2f_MACRO_GENERATED
#define ae2f_MAC() void
#endif

#include <ae2f/Float.h>
#include <stddef.h>

/** Quick Pattern */
#if CL_Q
#define CL_Q_CVRT(x)	x##_Q
#else
#define CL_Q_CVRT(x)	x
#endif

#endif
