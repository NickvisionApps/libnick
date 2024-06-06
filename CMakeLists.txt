cmake_minimum_required (VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)
    if(EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(USING_VCPKG ON)
    endif()
endif()
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#libnick Definition
project ("libnick" LANGUAGES C CXX VERSION 2024.6.2 DESCRIPTION "A cross-platform base for native Nickvision applications.")
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)

#libnick Setup
add_compile_definitions(SQLITE_HAS_CODEC)
if(LINUX)
    add_compile_definitions(HAVE_USLEEP)
endif()
add_library (${PROJECT_NAME}
    "src/app/appinfo.cpp"
    "src/app/aura.cpp"
    "src/app/configurationbase.cpp"
    "src/app/interprocesscommunicator.cpp"
    "src/app/windowgeometry.cpp"
    "src/database/sqlcontext.cpp"
    "src/database/sqldatabase.cpp"
    "src/database/sqlite3.c"
    "src/database/sqlstatement.cpp"
    "src/database/sqlvalue.cpp"
    "src/filesystem/filesystemchangedeventargs.cpp"
    "src/filesystem/filesystemwatcher.cpp"
    "src/filesystem/systemdirectories.cpp"
    "src/filesystem/userdirectories.cpp"
    "src/helpers/stringhelpers.cpp"
    "src/helpers/webhelpers.cpp"
    "src/keyring/credential.cpp"
    "src/keyring/keyring.cpp"
    "src/keyring/keyringdialogcontroller.cpp"
    "src/keyring/passwordgenerator.cpp"
    "src/keyring/passwordstrength.cpp"
    "src/keyring/store.cpp"
    "src/keyring/systemcredentials.cpp"
    "src/localization/gettext.cpp"
    "src/logging/logger.cpp"
    "src/network/networkmonitor.cpp"
    "src/network/networkstatechangedeventargs.cpp"
    "src/notifications/notificationsenteventargs.cpp"
    "src/notifications/notifyicon.cpp"
    "src/notifications/notifyiconmenu.cpp"
    "src/notifications/shellnotification.cpp"
    "src/notifications/shellnotificationsenteventargs.cpp"
    "src/system/environment.cpp"
    "src/system/process.cpp"
    "src/system/processexitedeventargs.cpp"
    "src/taskbar/taskbaritem.cpp"
    "src/update/updater.cpp"
    "src/update/version.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_VERSION}")
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${PROJECT_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

#libnick Packages
find_package(CURL REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(Intl REQUIRED)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl JsonCpp::JsonCpp Intl::Intl OpenSSL::SSL OpenSSL::Crypto)
if(USING_VCPKG)
    find_package(unofficial-maddy CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::maddy::maddy)
endif()
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC Advapi32 Dwmapi Gdiplus Kernel32 Shell32 UxTheme)
elseif(LINUX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
    pkg_check_modules(gio REQUIRED IMPORTED_TARGET gio-2.0)
    pkg_check_modules(gmodule REQUIRED IMPORTED_TARGET gmodule-2.0)
    pkg_check_modules(gobject REQUIRED IMPORTED_TARGET gobject-2.0)
    pkg_check_modules(gthread REQUIRED IMPORTED_TARGET gthread-2.0)
    pkg_check_modules(libsecret REQUIRED IMPORTED_TARGET libsecret-1)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads PkgConfig::glib PkgConfig::gio PkgConfig::gmodule PkgConfig::gobject PkgConfig::gthread PkgConfig::libsecret)
    if(USING_VCPKG)
        find_package(unofficial-libuuid CONFIG REQUIRED)
        target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::UUID::uuid)
    else()
        find_package(Libuuid REQUIRED)
        if(Libuuid_FOUND)
            target_include_directories(${PROJECT_NAME} INTERFACE ${Libuuid_INCLUDE_DIRS})
            target_link_libraries(${PROJECT_NAME} INTERFACE ${Libuuid_LIBRARIES})
        endif()
    endif()
endif()

#libnick Install
configure_file("${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}.pc" @ONLY)
configure_package_config_file("${CMAKE_SOURCE_DIR}/cmake/config.cmake.in" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
write_basic_package_version_file("${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake" VERSION "${PROJECT_VERSION}" COMPATIBILITY AnyNewerVersion)
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}Targets" LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(FILES "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
install(EXPORT "${PROJECT_NAME}Targets" FILE "${PROJECT_NAME}Targets.cmake" NAMESPACE ${PROJECT_NAME}:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(FILES "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake" "${CMAKE_SOURCE_DIR}/cmake/FindLibuuid.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
export(EXPORT "${PROJECT_NAME}Targets" FILE "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake" NAMESPACE ${PROJECT_NAME}::)

#libnick Test
if (BUILD_TESTING)
	add_executable(${PROJECT_NAME}_test
        "tests/auratests.cpp"
        "tests/databasetests.cpp"
        "tests/eventtests.cpp"
        "tests/filewatchertests.cpp"
        "tests/ipctests.cpp"
        "tests/keyringtests.cpp"
        "tests/loggingtests.cpp"
        "tests/main.cpp"
        "tests/networktests.cpp"
        "tests/notifyicontests.cpp"
        "tests/passwordtests.cpp"
        "tests/storetests.cpp"
        "tests/stringtests.cpp"
        "tests/systemcredentialstests.cpp"
        "tests/systemtests.cpp"
        "tests/taskbartests.cpp"
        "tests/updatertests.cpp"
        "tests/versiontests.cpp"
        "tests/webtests.cpp")
	find_package(GTest CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${PROJECT_NAME})
endif()
