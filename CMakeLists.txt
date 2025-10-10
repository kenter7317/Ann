cmake_minimum_required(VERSION 3.20)
enable_testing()
option(ae2f_CXX "" ON)

option(ae2fVK_needed "" OFF)
option(ae2fVK_need_clspv "" OFF)
option(ae2f_TEST "" ON)
option(ae2f_MAC_BUILD "" ON)
option(ae2f_IS_SHARED "" ON)
option(ae2f_DEVEL ""	ON)

include(cmake/Core.cmake)

# Soon will be doomed
if(NOT WIN32 AND ${ae2f_TEST})
	link_libraries(m)
endif()

project(MMap VERSION 0.0.0 LANGUAGES C CXX)

if(NOT TARGET ae2f::MMap)
	ae2f_CoreLibFetchX(ae2f Core	main)
	ae2f_CoreLibFetchX(ae2f Preproc	main)

	file(GLOB mmap-inc ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/*.h)
	configure_file(${PROJECT_SOURCE_DIR}/src/mmap/Util.h.in ${PROJECT_SOURCE_DIR}/inc/ae2f/MMap/Util.h)

	ae2f_Macro_init(1 100 100)

	ae2f_Macro_Lib_V(
    ae2f MMap INTERFACE
		${PROJECT_SOURCE_DIR}/src/mmap "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/mmap "*.def.c" 
		${PROJECT_SOURCE_DIR}/inc/ae2f/MMap ".auto.h"
		${PROJECT_SOURCE_DIR}/Config/MMap.cmake.in inc
		${mmap-inc}
		)

  target_link_libraries(${ae2f__MMap__TENT} INTERFACE ae2f::Core)
  target_link_libraries(${ae2f__MMap__TENT} INTERFACE ae2f::Preproc)
endif()

project(Ann VERSION 0.0.0 LANGUAGES C CXX)
if(NOT TARGET ae2f::Ann)
	file(GLOB Ann-inc ${PROJECT_SOURCE_DIR}/inc/ae2f/Ann/*.h)

	ae2f_Macro_Lib_V(
    ae2f Ann INTERFACE
		${PROJECT_SOURCE_DIR}/src/ann "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/ann "*.def.c*" 
		${PROJECT_SOURCE_DIR}/inc/ae2f/Ann ".auto.h"
		${PROJECT_SOURCE_DIR}/Config/Ann.cmake.in inc
		${Ann-inc}
		)

    target_link_libraries(${ae2f__Ann__TENT} INTERFACE ${ae2f__MMap__TENT})
	ae2f_CoreTestTent(${ae2f__Ann__TENT} "test/ann")
endif()

if(ae2fVK_needed AND NOT TARGET ae2fVK::Ann)
	ae2f_CoreLibFetchX(ae2f VK-Core	main)
	file(GLOB AnnVK-inc ${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/*.h)

	if(ae2f_DEVEL)
		file(GLOB_RECURSE AnnVK-src ${PROJECT_SOURCE_DIR}/src/vk/*.inc.c*)
	else()
		set(AnnVK-src "")
	endif()

	ae2f_Macro_init(1 100 100)

	ae2f_Inc_init(
		100 100 10 ON ON ON
		"-DINC_LEAVE_NFOUND=OFF"
		"-DNDEBUG=ON"
		)

	get_target_property(Core_INCLUDE_DIRS ae2f::Core INCLUDE_DIRECTORIES)
	get_target_property(VKCore_INCLUDE_DIRS ae2fVK::Core INCLUDE_DIRECTORIES)
	message("Dir: ${Core_INCLUDE_DIRS}/")

	ae2f_FileRaw_init(ON 1 -DNDEBUG=ON)


	ae2f_Macro_one(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.h
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.h
		)

	ae2f_Macro_one(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.h
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.h
		)

	# SLP Shader configuring
	ae2f_Macro_one(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.cl.c
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.cl.i
		)

	ae2f_Inc_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.cl.i
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.cl
		"${Core_INCLUDE_DIRS}/"
		"${VKCore_INCLUDE_DIRS}/"
		"${PROJECT_SOURCE_DIR}/inc/"
		)

	ae2f_FileRaw_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.cl
		${PROJECT_SOURCE_DIR}/src/vk/cl/Slp.auto.cl.impl
		)
	# SLP Shader configuring end

	# MLP Shader configuring
	ae2f_Macro_one(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.cl.c
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.cl.i
		)

	ae2f_Inc_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.cl.i
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.cl
		"${Core_INCLUDE_DIRS}/"
		"${VKCore_INCLUDE_DIRS}/"
		"${PROJECT_SOURCE_DIR}/inc/"
		)

	ae2f_FileRaw_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.cl
		${PROJECT_SOURCE_DIR}/src/vk/cl/Mlp.auto.cl.impl
		)
	# MLP Shader configuring end

	ae2f_Inc_init(
		100 100 10 ON ON ON
		"-DINC_LEAVE_NFOUND=ON"
		"-DNDEBUG=ON"
		)

	ae2f_Inc_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/SlpSHADER.inc.c
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/SlpSHADER.auto.h
		)

	ae2f_Inc_Run_One(
		${PROJECT_SOURCE_DIR}/src/vk/MlpSHADER.inc.c
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/MlpSHADER.auto.h
		)

	ae2f_Macro_Lib_V(
    ae2fVK Ann INTERFACE 
		${PROJECT_SOURCE_DIR}/src/vk "*.imp.c" 
		${PROJECT_SOURCE_DIR}/src/vk "*.def.c*" 
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann ".auto.h"
		${PROJECT_SOURCE_DIR}/Config/VK-Ann.cmake.in inc
		${AnnVK-inc} ${AnnVK-src}
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/Mlp.core.h
		${PROJECT_SOURCE_DIR}/inc/ae2fVK/Ann/Slp.core.h
		)

    target_link_libraries(${ae2fVK__Ann__TENT} INTERFACE ae2f::Ann)
    target_link_libraries(${ae2fVK__Ann__TENT} INTERFACE ae2fVK::Core)
	ae2f_CoreTestTent(${ae2fVK__Ann__TENT} "test/vk")
endif()
