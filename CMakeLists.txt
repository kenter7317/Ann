cmake_minimum_required(VERSION 3.20)
enable_testing()
option(ae2f_CXX "" ON)
option(ae2fVK_needed "" OFF)
option(ae2f_TEST "" ON)
option(ae2f_MAC_BUILD "" ON)
option(ae2f_IS_SHARED "" OFF)
option(ae2f_DEVEL ""	OFF)

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

if(NOT TARGET ae2f::MMap)
	ae2f_CoreLibFetchX(ae2f Core	main)
	ae2f_CoreLibFetchX(ae2f Preproc	main)

	file(GLOB mmap-inc ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/*.h)
	configure_file(${PROJECT_SOURCE_DIR}/src/mmap/Util.h.in ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/Util.h)

	ae2f_Macro_init(1 100 100)

	ae2f_Macro_Lib_V(
		ae2f MMap "${ae2f_LIBPREFIX}" 
		${PROJECT_SOURCE_DIR}/src/mmap "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/mmap "*.def.c" 
		${PROJECT_SOURCE_DIR}/inc/ae2f/MMap ".h"
		${PROJECT_SOURCE_DIR}/Config/MMap.cmake.in inc
		${mmap-inc}
	)

	ae2f_itll(${ae2f__MMap__TENT} ae2f::Core)
	ae2f_itll(${ae2f__MMap__TENT} ae2f::Preproc)
endif()

project(Ann VERSION 0.0.0 LANGUAGES C CXX)
if(NOT TARGET ae2f::Ann)
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

if(ae2fVK_needed AND NOT TARGET ae2fVK::Ann)
	ae2f_CoreLibFetchX(ae2f VK-Core	main)
	file(GLOB AnnVK-inc ${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/*.h)

	if(ae2f_DEVEL)
		file(GLOB_RECURSE AnnVK-src ${PROJECT_SOURCE_DIR}/src/vk/*.def.c*)
	else()
		set(AnnVK-src "")
	endif()

	ae2f_Macro_Lib_V(
		ae2fVK VK-Ann "${ae2f_LIBPREFIX}" 
		${PROJECT_SOURCE_DIR}/src/vk "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/vk "*.def.c*" 
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann ".h"
		${PROJECT_SOURCE_DIR}/Config/VK-Ann.cmake.in inc
		${AnnVK-inc} ${AnnVK-src}
	)

	add_library(ae2fVK::Ann ALIAS ${ae2fVK__VK-Ann__TENT})
	add_library(ae2f::VK-Ann ALIAS ${ae2fVK__VK-Ann__TENT})

	ae2f_itll(${ae2fVK__VK-Ann__TENT} ae2f::Ann)
	ae2f_itll(${ae2fVK__VK-Ann__TENT} ae2fVK::Core)

	ae2f_CoreTestTent(${ae2fVK__VK-Ann__TENT} "test/vk")
endif()
