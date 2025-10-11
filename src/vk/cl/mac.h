#ifndef mac_h
#define mac_h

#define ae2f_MAC()	void
#define	CAST(t, x)	((t)(x))


/** Quick Pattern */
#if CL_Q
#define CL_Q_CVRT(x)	x##_Q
#else
#define CL_Q_CVRT(x)	x
#endif

#include <ae2fVK/clspv/addrspec.h>
#include <ae2fVK/clspv/wrkitem.h>
#include <ae2fVK/clspv/atom.h>

#if ae2fVK_CL(!)0
#define work_group_reduce_add(v)	0
#endif


#endif
