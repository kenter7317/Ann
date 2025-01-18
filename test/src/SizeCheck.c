#include <ae2fCL/Ann.h>
#include "../test.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include <ae2f/Float.h>
#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>

#include <CL/cl.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>

int main() {
    if(!ae2fCL_eAnnKerns_LEN) {
        puts("[ERR] Kernels are not checked as enum.");
        return 1;
    }

    #define __sizematch(type) \
    puts("[LOG] Test: " #type); \
    if(sizeof(type) != type##_SIZE) { \
        puts("[ERR] {" #type "} size error"); \
        return 1; \
    }

    __sizematch(ae2f_float_t);
    __sizematch(cl_mem);
    

    puts("[GOOD] Size Check is done gracefully.");
    return 0;
}