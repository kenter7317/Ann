#ifndef ae2f_Ann_Util_h
#define ae2f_Ann_Util_h

#include <ae2f/Pack/Beg.h>
#include <ae2f/MMap/Util.h>

#define ae2f_AnnUtilPtr2Name(A_t, B_t) \
	ae2f_AnnUtilPtr__##A_t##B_t

#define ae2f_AnnUtilV2Name(A_t, B_t) \
	ae2f_AnnUtilV2Name__##A_t##B_t

#define ae2f_AnnUtilV2(A_LEFT, A_t, A_POST, B_LEFT, B_t, B_POST) \
	typedef union { A_LEFT A_t A_POST a; B_LEFT B_t B_POST b; } ae2f_AnnUtilV2Name(A_t, B_t)

#define ae2f_AnnUtilPtr2(A_t, B_t, ...) \
	typedef union { A_t* a; B_t* b; } ae2f_AnnUtilPtr2Name(A_t, B_t)

#define ae2f_AnnUtilIdx1(idx0, sz0)				(idx0)
#define ae2f_AnnUtilIdx2(idx1, sz1, idx0, sz0)			((idx1) * (sz0) + (idx0))
#define ae2f_AnnUtilIdx3(idx2, sz2, idx1, sz1, idx0, sz0)	((idx2) * (sz1) * (sz0) + (idx1) * (sz0) + (idx0))

#include <ae2f/Pack/End.h>

#endif
