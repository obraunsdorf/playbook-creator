find_package (Boost REQUIRED COMPONENTS unit_test_framework filesystem)
include_directories (${Boost_INCLUDE_DIRS})

find_package( Botan 1.10 REQUIRED )
include_directories(${BOTAN_INCLUDE_DIR})

include_directories(../src)
add_executable (tests tests.cpp)
target_link_libraries (tests PBCLib ${Boost_LIBRARIES})