cmake_minimum_required(VERSION 3.20)
enable_testing()
option(ae2f_CXX "" ON)
option(ae2fCL_needed "" ON)
option(ae2fVK_needed "" ON)
option(ae2f_TEST "" ON)
include(cmake/Core.cmake)

# Soon will be doomed
    if(NOT _WIN32)
	    link_libraries(m)
    endif()

project(MMap VERSION 0.0.0 LANGUAGES C CXX)
if(NOT TARGET ${PROJECT_NAME})
	ae2f_CoreLibFetchX(ae2f Core Rel-v6.23.17)

	ae2f_CoreLibTentConfigCustom(
		MMap-H INTERFACE
		include ae2f
		${PROJECT_SOURCE_DIR}/Config/MMap-H.cmake.in
		"include/ae2f/MMap/MMap.h"
		"include/ae2f/MMap/MMap.x/MMap.hh"
		"include/ae2f/MMap/MMap.x/mMMap.hh"
		"include/ae2f/MMap/MMap.x/imp.hh"
	)

    message("LINKING: ${ae2f__Core__FETCHED}")
	target_link_libraries(MMap-H INTERFACE ${ae2f__Core__FETCHED})

	ae2f_CoreLibTentConfigCustom(
		MMap ${ae2f_LIBPREFIX}
		include ae2f
		${PROJECT_SOURCE_DIR}/Config/MMap.cmake.in
		"src/mmap/MMap.c"
	)

	target_link_libraries(MMap PUBLIC MMap-H)
endif()

project(Ann VERSION 0.0.0 LANGUAGES C CXX)
if(NOT TARGET ${PROJECT_NAME})
    ae2f_CoreLibFetchX(ae2f MMap-H 0.0.0)
    ae2f_CoreLibFetchX(ae2f MMap 0.0.0)

    ae2f_CoreLibTentConfigCustom(
        Ann-H INTERFACE
        include ae2f
        ${PROJECT_SOURCE_DIR}/Config/Ann-H.cmake.in 
        "include/ae2f/Ann/Act.h"
        "include/ae2f/Ann/LcgRand.h"
        
        "include/ae2f/Ann/Conv.h"
        
        "include/ae2f/Ann/Sp.h"
        "include/ae2f/Ann/Sp.h.c/clean.script.h"
        "include/ae2f/Ann/Sp.h.c/cleandef.h"
        "include/ae2f/Ann/Sp.h.cxx/Sp.hh"
        "include/ae2f/Ann/Sp.h.cxx/mSp.hh"
        "include/ae2f/Ann/Sp.h.cxx/imp.hh"

        "include/ae2f/Ann/Slp.h"
        "include/ae2f/Ann/Slp.h.cxx/Slp.hh"
        "include/ae2f/Ann/Slp.h.cxx/mSlp.hh"
        "include/ae2f/Ann/Slp.h.cxx/imp.hh"

        "include/ae2f/Ann/Mlp.h"
        "include/ae2f/Ann/Mlp.h.cxx/Mlp.hh"
        "include/ae2f/Ann/Mlp.h.cxx/mMlp.hh"
        "include/ae2f/Ann/Mlp.h.cxx/imp.hh"
    )

    message("LINKING: ${ae2f__MMap-H__FETCHED}")
    message("LINKING: ${ae2f__MMap__FETCHED}")
    target_link_libraries(Ann-H INTERFACE ${ae2f__MMap-H__FETCHED})

    ae2f_CoreLibTentConfigCustom(
        Ann-H-Private INTERFACE
        src ae2f
        ${PROJECT_SOURCE_DIR}/Config/Ann-H-Private.cmake.in

        "src/ann-h/Sp.h"
        "src/ann-h/Slp.h"
        "src/ann-h/Mlp.h"
        "src/ann-h/Mlp/Predict.h"
        "src/ann-h/Mlp/OutCompute.h"
        "src/ann-h/Mlp/HidErr.h"
        "src/ann-h/Mlp/HidCompute.h"
    )
    target_link_libraries(Ann-H-Private INTERFACE Ann-H)

    ae2f_CoreLibTentConfigCustom(
        Ann ${ae2f_LIBPREFIX}
        include ae2f
        ${PROJECT_SOURCE_DIR}/Config/Ann.cmake.in
        "src/ann/LcgRand.c"
        "src/ann/Sp.c"
        "src/ann/Slp.c"
        "src/ann/Mlp.c"
	    "src/ann/Conv.c"
    )

    target_link_libraries(Ann PRIVATE Ann-H-Private)
    target_link_libraries(Ann PUBLIC Ann-H)
    target_link_libraries(Ann PUBLIC ${ae2f__MMap__FETCHED})

    if(NOT WIN32)
        target_link_libraries(Ann PUBLIC m)
    endif()

    ae2f_CoreTestTent(Ann "test/src")
endif()

if(ae2fCL_needed)
    project(CL-Ann VERSION 3.3.4 LANGUAGES C CXX)
    if(NOT TARGET ${PROJECT_NAME})
    ae2f_CoreLibFetchX(ae2f Ruler		Rel-v0.1.5)
    ae2f_CoreLibFetchX(ae2f Ann 		None)
    ae2f_CoreLibFetchX(ae2f Ann-H 		None)
    ae2f_CoreLibFetchX(ae2f Ann-H-Private 	None)
    find_package(OpenCL REQUIRED)

    ae2f_CoreLibFetchX_NS(ae2f ae2fCL CL-Core	Rel-v4.15.21)

        ae2f_CoreLibTentConfigCustom(
            CL-Ann-H INTERFACE
            include ae2fCL
            ${PROJECT_SOURCE_DIR}/Config/CL-Ann-H.cmake.in
            "include/ae2fCL/Ann.h"
            
            "include/ae2fCL/Ann/Sp.h.cxx/Sp.hh"
            "include/ae2fCL/Ann/Sp.h.cxx/mSp.hh"
            "include/ae2fCL/Ann/Sp.h.cxx/imp.hh"
            "include/ae2fCL/Ann/Sp.h"

            "include/ae2fCL/Ann/Slp.h.cxx/Slp.hh"
            "include/ae2fCL/Ann/Slp.h.cxx/mSlp.hh"
            "include/ae2fCL/Ann/Slp.h.cxx/imp.hh"
            "include/ae2fCL/Ann/Slp.h"
        )

        ae2f_RulerMeasure(
            cl_mem 
            ${PROJECT_SOURCE_DIR}/src/cl/CLCode/Size/cl_mem.auto.h 
            -I${OpenCL_INCLUDE_DIR} 
            CL/cl.h
        )

        ae2f_RulerMeasure(
            ae2f_float_t 
            ${PROJECT_SOURCE_DIR}/src/cl/CLCode/Size/ae2f_float_t.auto.h 
            -I${PROJECT_SOURCE_DIR}/submod/ae2f/Core/include/ae2f
            Float.auto.h
        )

        ae2f_CoreLibTentConfigCustom(
            CL-Ann ${ae2f_LIBPREFIX}
            include ae2fCL
            ${PROJECT_SOURCE_DIR}/Config/CL-Ann.cmake.in
            "src/cl/Ann.c"
            "src/cl/Sp.c"
            "src/cl/Slp.c"
            "src/cl/Mlp.c"
            "src/cl/CLCode/Size/ae2f_float_t.auto.h"
            "src/cl/CLCode/Size/cl_mem.auto.h"
        )

	target_link_libraries(CL-Ann-H INTERFACE ${ae2f__Ann-H__FETCHED})
	target_link_libraries(CL-Ann-H INTERFACE ${ae2f__ae2fCL__CL-Core__FETCHED})
	target_link_libraries(CL-Ann PUBLIC CL-Ann-H)
	target_link_libraries(CL-Ann PUBLIC ${ae2f__Ann-H-Private__FETCHED})
	target_link_libraries(CL-Ann PUBLIC ${ae2f__Ann__FETCHED})
	target_link_libraries(CL-Ann PUBLIC OpenCL::OpenCL)

        ae2fCL_CoreAppendInclude(
            ${PROJECT_SOURCE_DIR}/include
            ${ae2f_ProjRoot}/submod/ae2f/Core/include
            ${ae2f_ProjRoot}/submod/ae2f/CL-Core/include
        )
        message(${OpenCL_INCLUDE_DIR}/CL/cl.h)
        
        ae2fCL_CoreAddConfProjTarDep(
            CL-Ann
            "${PROJECT_SOURCE_DIR}/src/cl/CLCode"
        )

        ae2fCL_CoreTestTent(CL-Ann "test/cl-src")

    endif()
endif()

if(ae2fVK_needed)
	project(VK-Ann VERSION 0.0.0 LANGUAGES C CXX)

	if(NOT TARGET ${PROJECT_NAME})
		ae2f_CoreLibFetchX(ae2f Ruler		Rel-v0.1.5)
		ae2f_CoreLibFetchX(ae2f Ann 		None)
		ae2f_CoreLibFetchX(ae2f Ann-H 		None)
		ae2f_CoreLibFetchX(ae2f Ann-H-Private 	None)
		find_package(Vulkan REQUIRED)
		ae2f_CoreLibFetchX_NS(ae2f ae2fVK VK-Core	Rel-v5.15.22)

		file(GLOB_RECURSE VK-Ann-src ${PROJECT_SOURCE_DIR}/src/vk/*)
		file(GLOB_RECURSE VK-Ann-inc ${PROJECT_SOURCE_DIR}/include/ae2fVK/*)

        ae2f_CoreLibTentConfigCustom(
            VK-Ann-H INTERFACE
                include ae2fVK
                ${PROJECT_SOURCE_DIR}/Config/VK-Ann-H.cmake.in
            ${VK-Ann-inc}
        )

        ae2f_CoreLibTentConfigCustom(
            VK-Ann ${ae2f_LIBPREFIX}
            include ae2fVK
            ${PROJECT_SOURCE_DIR}/Config/VK-Ann.cmake.in
            ${VK-Ann-src} ${VK-Ann-inc}
        )
	endif()
endif()
