cmake_minimum_required (VERSION 3.22)
set(CMAKE_CXX_STANDARD 14)

project (PlaybookCreator)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

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

# Fetch and setup Corrosion for Rust integration
include(FetchContent)
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.6
)
FetchContent_MakeAvailable(Corrosion)

add_subdirectory(3rdparty/Qt-Color-Widgets)

add_subdirectory(src)
add_subdirectory(test)


add_custom_target( documentation COMMAND doxygen)
