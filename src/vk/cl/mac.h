#ifndef mac_h
#define mac_h

#if !__ae2f_MACRO_GENERATED

#define ae2f_MAC() void

#endif

#include <ae2f/Float.h>

#ifndef ACT
#define ACT(x)		(x)
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(x)	(x)
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(y, y_desired, i, c)
#endif

#include <stddef.h>

#endif
