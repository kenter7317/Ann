#ifndef mac_h
#define mac_h

#include <ae2fVK/clspv/addrspec.h>
#include <ae2fVK/clspv/wrkitem.h>
#include <ae2fVK/clspv/atom.h>
#include <ae2fVK/clspv/stdint.h>

#define host_float_t ae2f_float

#ifdef ae2f_float_t
#define ae2f_Float_h
#include <ae2f/Float.auto.h>
#endif

#define	CAST(t, x)	((t)(x))
#define PTRCAST(t, x, pad)	CAST(t, CAST(intptr_t, x) + (pad))
#define MAX(a, b)	((a) < (b) ? (b) : (a))

#define size_t		uint32_t

/** CL_Q is deprecated */
#define CL_Q_CVRT(x)	x

#endif

#undef	ae2f_MAC

#define ae2f_MAC(...)	void
