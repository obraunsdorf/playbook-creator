cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project (PlaybookCreator)

find_package(Git)
if(GIT_FOUND)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
	include( MunkeiVersionFromGit )
	version_from_git(
  		GIT_EXECUTABLE
  		INCLUDE_HASH True
	)
	message("Project version from git tag: ${GIT_TAG}")
	configure_file(cmake/pbcVersion.h.in src/pbcVersion.h)
endif()

#add_definitions(-DCMAKEVALUE 255)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)