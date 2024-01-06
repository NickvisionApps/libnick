vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NickvisionApps/libaura
    REF d058e7e6c1732d1fc1509450c59b38a189a32beb
    SHA512 85d9db4e93463b803c28ae783027a00d2a824b49d485cd1c29b54767ece6e1747d8c1e7f547bcfedf2f1199186ccc0cb3bdf7cb6d17a05860d5d75c0596ea1aa
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTING=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/libaura)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_copy_pdbs()

vcpkg_fixup_pkgconfig()

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)