/// @file Mlp.h

#ifndef ae2fCL_Ann_Mlp_h
#define ae2fCL_Ann_Mlp_h

#include "Slp.h"

typedef ae2fCL_AnnSlp ae2fCL_AnnMlpEl;
typedef struct ae2fCL_AnnMlp {
    ae2fCL_AnnMlpEl* List;
    size_t Count, MaxBuff;
} ae2fCL_AnnMlp;

#endif