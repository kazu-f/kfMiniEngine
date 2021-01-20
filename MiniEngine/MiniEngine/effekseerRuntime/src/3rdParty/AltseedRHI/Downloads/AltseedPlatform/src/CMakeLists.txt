cmake_minimum_required(VERSION 2.8)
project(AltseedPlatform CXX)


FILE(GLOB cpps *.cpp)
FILE(GLOB cpps_pc PC/*.cpp)
list(APPEND cpps ${cpps_pc})

FILE(GLOB hs *.h)
FILE(GLOB hs_pc PC/*.h)
list(APPEND hs ${hs_pc})

foreach(FILE ${hs}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()

foreach(FILE ${cpps}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()


# Flag
if (MSVC)
	if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
		foreach (flag CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE)

			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
endif()

set(common_lib)
if(MSVC)
	# do nothing
else()
	list(APPEND common_lib glfw3 )

	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		# used in GLFW
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(IOKIT_FRAMEWORK IOKit)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
		list(APPEND common_lib
			${COCOA_FRAMEWORK}
			${OPENGL_gl_LIBRARY}
			${OPENGL_glu_LIBRARY}
			${IOKIT_FRAMEWORK}
			${CORE_FOUNDATION_FRAMEWORK}
			${CORE_VIDEO_FRAMEWORK})
	endif()
endif()

add_library(AltseedPlatform ${cpps} ${hs})

target_link_libraries(AltseedPlatform ${common_lib}) 

include_directories(../include/)

set_target_properties(AltseedPlatform PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET AltseedPlatform PROPERTY CXX_STANDARD 11)
set_property(TARGET AltseedPlatform PROPERTY CXX_STANDARD_REQUIRED ON)