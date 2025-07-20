cmake_minimum_required(VERSION 3.20)
enable_testing()
option(ae2f_CXX "" ON)
option(ae2fCL_needed "" OFF)
option(ae2f_TEST "" ON)
option(ae2f_MAC_BUILD "" ON)
option(ae2f_IS_SHARED "" OFF)

include(cmake/Core.cmake)

# Soon will be doomed
if(NOT WIN32 AND ${ae2f_TEST})
	link_libraries(m)
endif()

project(MMap VERSION 0.0.0 LANGUAGES C CXX)

if(ae2f_MAC_BUILD)
	function(ae2f_itll tar)
		target_link_libraries(${tar} ${ARGN})
	endfunction()
else()
	function(ae2f_itll tar)
		target_link_libraries(${tar} INTERFACE ${ARGN})
	endfunction()

endif()

if(NOT TARGET ae2f::${PROJECT_NAME})
	ae2f_CoreLibFetchX(ae2f Core	main)
	ae2f_CoreLibFetchX(ae2f Macro	main)

	file(GLOB mmap-inc ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/*.h)
	configure_file(${PROJECT_SOURCE_DIR}/src/mmap/Util.h.in ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/Util.h)

	ae2f_Macro_init(1, 100, 100)

	ae2f_Macro_Lib_V(
		ae2f MMap "${ae2f_LIBPREFIX}" 
		${PROJECT_SOURCE_DIR}/src/mmap "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/mmap "*.def.c" 
		${PROJECT_SOURCE_DIR}/inc/ae2f/MMap ".h"

		${PROJECT_SOURCE_DIR}/Config/MMap.cmake.in inc
		${mmap-inc}
	)

	ae2f_itll(${ae2f__MMap__TENT} ${ae2f__Core__FETCHED})
	ae2f_itll(${ae2f__MMap__TENT} ${ae2f__Macro__FETCHED})
endif()

project(Ann VERSION 0.0.0 LANGUAGES C CXX)
if(NOT TARGET ae2f::${PROJECT_NAME})
	file(GLOB Ann-inc ${PROJECT_SOURCE_DIR}/inc/ae2f/Ann/*.h)

	ae2f_Macro_Lib_V(
		ae2f Ann "${ae2f_LIBPREFIX}" 
		${PROJECT_SOURCE_DIR}/src/ann "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/ann "*.def.c*" 
		${PROJECT_SOURCE_DIR}/inc/ae2f/Ann ".h"
		${PROJECT_SOURCE_DIR}/Config/Ann.cmake.in inc
		${Ann-inc}
	)

	ae2f_itll(${ae2f__Ann__TENT} ${ae2f__MMap__TENT})
	ae2f_CoreTestTent(${ae2f__Ann__TENT} "test/ann")
endif()

if(ae2fCL_needed)
	project(CL-Ann VERSION 3.3.4 LANGUAGES C CXX)
	if(NOT TARGET ${PROJECT_NAME})
		ae2f_CoreLibFetchX(ae2f Ruler		main)
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
			Float.h
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
