option(ae2f_IS_SHARED "Is a shared library or static one." OFF)
option(ae2f_DOC "When activated, it would generate project with the deaders of cmake utility functions." OFF)
option(ae2f_TEST "When activated, it would generate test projects." ON)
option(ae2f_CXX "Tell that thou art including cxx for thy project." OFF)

set(ae2f_float float CACHE STRING "Float type for the template.")
set(ae2f_packcount 0 CACHE STRING "Pack count for pre-defined structures.")
set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Current Source Root")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Current Binary Root")

if(ae2f_IS_SHARED)
    set(ae2f_LIBPREFIX SHARED CACHE STRING "SHARED")
else()
    set(ae2f_LIBPREFIX STATIC CACHE STRING "STATIC")
endif()

# @brief 
# It will try to fetch the cmake project ae2f-Core like project for Local and Github. \n
# @see ___DOC_CMAKE::ae2f_LibDirGlob is the given path to check. \n 
# 
# Once the project is in given directory, it will not try to fetch it from internet.
# 
# Fethced Library will be in: ${prm_AuthorName}__${prm_TarName}__FETCHED 
# 
# @param prm_AuthorName 
# Author name
# @param prm_TarName
# Target name 
# @param prm_TagName
# Tag name
function(ae2f_CoreLibFetch prm_AuthorName prm_TarName prm_TagName)
    if(NOT TARGET ${prm_TarName})
	    if(NOT EXISTS ${ae2f_ProjRoot}/submod/${prm_AuthorName}/${prm_TarName}/CMakeLists.txt)
	        execute_process(
	            COMMAND 
	            git clone 
	            https://github.com/${prm_AuthorName}/${prm_TarName} 
	            ${ae2f_ProjRoot}/submod/${prm_AuthorName}/${prm_TarName}
	            --branch ${prm_TagName}
	            RESULT_VARIABLE result
	        )
	
	        if(result)
	            message(FATAL_ERROR "Fetching ${prm_AuthorName}/${prm_TarName} from Github Failed.")
	        endif()
	    endif()
    
		add_subdirectory(
		    ${ae2f_ProjRoot}/submod/${prm_AuthorName}/${prm_TarName}
		    ${ae2f_BinRoot}/submod/${prm_AuthorName}/${prm_TarName}
		)
    endif()

    set(${prm_AuthorName}__${prm_TarName}__FETCHED ${prm_TarName} CACHE STRING ${prm_TarName})
endfunction()


# Fetched library will be in ${prm_AuthorName}__${prm_TarName}__FETCHED 
function(ae2f_CoreLibFetchX prm_AuthorName prm_TarName prm_TagName)
	find_package(${prm_TarName})

	if(${prm_TarName}_FOUND)
		set(${prm_AuthorName}__${prm_TarName}__FETCHED ${prm_AuthorName}::${prm_TarName} CACHE STRING ${prm_AuthorName}::${prm_TarName})
	else()
		ae2f_CoreLibFetch(${prm_AuthorName} ${prm_TarName} ${prm_TagName})
	endif()
endfunction()