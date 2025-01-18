#define CHECK_IF(err, tarOkay) if(err != tarOkay) 
#define CHECK_ERR(err, tarOkay, __failure) CHECK_IF(err, tarOkay) { goto __failure; }

#include <math.h>