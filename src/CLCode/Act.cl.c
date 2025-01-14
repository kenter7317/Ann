#include "Act.h"

ae2f_float_t __ae2fCL_eAnnActSigmoid(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}