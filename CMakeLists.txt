cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project (PlaybookCreator)

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
		message(INFO: "no git repository")
	else()
		include( MunkeiVersionFromGit )
		version_from_git(
	  		GIT_EXECUTABLE
	  		INCLUDE_HASH True
		)
		message("Project version from git tag: ${GIT_TAG}")
		configure_file(cmake/pbcVersion.h.in src/pbcVersion.h)
	endif()
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)

add_custom_target( documentation COMMAND doxygen)