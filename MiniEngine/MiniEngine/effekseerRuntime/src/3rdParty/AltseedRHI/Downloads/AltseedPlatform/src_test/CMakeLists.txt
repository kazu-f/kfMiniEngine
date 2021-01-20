cmake_minimum_required(VERSION 2.8)
project(AltseedPlatform_Test CXX)

FILE(GLOB_RECURSE cpps *.cpp)
FILE(GLOB_RECURSE hs *.h)

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

link_directories(${PROJECT_SOURCE_DIR}/../lib/)
include_directories(
../include/
)



add_executable(AltseedPlatform_Test ${cpps} ${hs})
set_target_properties(AltseedPlatform_Test PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET AltseedPlatform_Test PROPERTY CXX_STANDARD 11)
set_property(TARGET AltseedPlatform_Test PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(AltseedPlatform_Test LINK_PUBLIC AltseedPlatform)

if(MSVC)

else()
target_link_libraries(AltseedPlatform_Test LINK_PUBLIC glfw3)
endif()

