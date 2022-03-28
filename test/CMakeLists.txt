set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package (Boost REQUIRED COMPONENTS unit_test_framework filesystem)
include_directories (${Boost_INCLUDE_DIRS})

find_package( Botan 1.10 REQUIRED )
include_directories(${BOTAN_INCLUDE_DIR})

# link Microsoft Visual Studio Runtime statically by default (on Windows builds)
#include(ConfigureMSVCRT)
#configure_msvc_runtime()

include_directories(../src)
add_executable (tests tests.cpp)

if(WIN32)
	# link msvc runtime statically WITHOUT debug symbols
	set_property(TARGET tests PROPERTY
 	 MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

target_link_libraries (tests ${MODE} PBCLib ${Boost_LIBRARIES})