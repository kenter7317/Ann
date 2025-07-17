#ifndef ae2f_Ann_Util_h
#define ae2f_Ann_Util_h

#include <ae2f/Cast.h>
#include <ae2f/Pack/Beg.h>


#define ae2f_AnnUtilPtr2Name(A_t, B_t) \
	ae2f_AnnUtilPtr__##A_t##B_t


#define ae2f_AnnUtilPtr2(A_t, B_t) \
	ae2f_structdef(union, ae2f_AnnUtilPtr2Name(A_t, B_t)) \
{ A_t* a; B_t* b; }

#include <ae2f/Pack/End.h>

#endif
