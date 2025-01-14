#include <ae2fCL/Ann.h>
#include "../test.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

int main() {

    if(!ae2fCL_eAnnKerns_LEN) {
        puts("[ERR] Kernels are not checked as enum.");
        return 1;
    }

    __PTRDIFF_WIDTH__;

    #if 1 == 22 ? 3 : 0
    #endif

    puts("[GOOD] Size Check is done gracefully.");
    return 0;
}