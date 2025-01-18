cmake_minimum_required(VERSION 3.20)
enable_testing()
include(cmake/Core.cmake)

project(CL-Ann VERSION 0.1.5 LANGUAGES C CXX)
if(NOT TARGET CL-Ann)
    ae2f_CoreLibFetch(ae2f CL-Core Rel-v4.9.14)
    ae2f_CoreLibFetch(ae2f Ruler Rel-v0.1.3)

    ae2f_CoreLibTent(
        CL-Ann ${ae2f_LIBPREFIX}
        include ae2fCL
        "include/ae2fCL/Ann/Sizes/cl_mem.h"

        "include/ae2fCL/Ann.h"
        "src/Ann.c"
        
        "include/ae2fCL/Ann/LcgRand.h"
        "src/LcgRand.c"
        
        "include/ae2fCL/Ann/Perc.h"
        "src/Perc.c"
    )
    
    target_link_libraries(CL-Ann PUBLIC CL-Core)
    if(NOT WIN32)
        target_link_libraries(CL-Ann PUBLIC m)
    endif()

    ae2fCL_CoreAppendInclude(
        ${PROJECT_SOURCE_DIR}/include
        ${ae2f_ProjRoot}/submod/ae2f/Core/include
        ${ae2f_ProjRoot}/submod/ae2f/CL-Core/include
    )
    message(${OpenCL_INCLUDE_DIR}/CL/cl.h)
    
    ae2fCL_CoreAddConfProjTarDep(
        CL-Ann
        "${PROJECT_SOURCE_DIR}/src"
    )
    ae2f_CoreTestTent(CL-Ann test/src)
    ae2f_RulerMeasure(cl_mem ${PROJECT_SOURCE_DIR}/include/ae2fCL/Ann/Sizes/cl_mem.h -I${OpenCL_INCLUDE_DIR} CL/cl.h)
    ae2f_RulerMeasure(ae2f_float_t ${PROJECT_SOURCE_DIR}/include/ae2fCL/Ann/Sizes/ae2f_float_t.h -I${PROJECT_SOURCE_DIR}/submod/ae2f/Core/include/ae2f Float.h)
endif()