cargo_build(NAME ${PBC_UPDATER_CMAKE_NAME})

# generate updater bindings
add_custom_command(
        OUTPUT  pbc_updater_bindings.h pbc_updater_bindings.cpp
        COMMAND cxxbridge ${PBC_UPDATER_PATH}/src/lib.rs --header > pbc_updater_bindings.h
        COMMAND cxxbridge ${PBC_UPDATER_PATH}/src/lib.rs > pbc_updater_bindings.cpp
        WORKING_DIRECTORY ${PBC_UPDATER_PATH}
        COMMENT "running dtolnay's cxx"
)

