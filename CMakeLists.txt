cmake_minimum_required (VERSION 3.15)
set(CMAKE_CXX_STANDARD 11)
project (PlaybookCreator)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Git)
if(GIT_FOUND)
	execute_process(
	    COMMAND           "${GIT_EXECUTABLE}" status
	    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	    RESULT_VARIABLE   git_result
	    OUTPUT_VARIABLE   git_describe
	    ERROR_VARIABLE    git_error
	    OUTPUT_STRIP_TRAILING_WHITESPACE
	    ERROR_STRIP_TRAILING_WHITESPACE
	    )
	if( NOT git_result EQUAL 0 )
		message(WARNING "no git repository")
	else()
		include( MunkeiVersionFromGit )
		version_from_git(
	  		GIT_EXECUTABLE
	  		INCLUDE_HASH True
		)
		message(STATUS "Project version from git tag: ${GIT_TAG}")
		configure_file(src/pbcVersion.h.in ${PROJECT_SOURCE_DIR}/src/pbcVersion.h)
	endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


enable_language(Rust)
include(CMakeCargo)
set(PBC_UPDATER_CMAKE_NAME pbc-updater)
add_subdirectory(updater)

set(PBC_RUST_HELPERS_CMAKE_NAME rust-helpers)
add_subdirectory(rust-helpers)

add_subdirectory(3rdparty/Qt-Color-Widgets)

add_subdirectory(src)
add_subdirectory(test)


add_custom_target( documentation COMMAND doxygen)
