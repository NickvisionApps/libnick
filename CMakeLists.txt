cmake_minimum_required (VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("libaura" VERSION 2023.12.0 DESCRIPTION "A cross-platform base for native Nickvision applications.")
include(GNUInstallDirs)

#libaura Library Setup
include_directories(${PROJECT_SOURCE_DIR}/include)
add_library (${PROJECT_NAME} SHARED
	src/events/eventargs.cpp
	src/events/notificationsenteventargs.cpp
	src/events/shellnotificationsenteventargs.cpp
	src/helpers/stringhelpers.cpp
	src/helpers/webhelpers.cpp
	src/keyring/credential.cpp
	src/keyring/passwordgenerator.cpp
	src/update/updater.cpp
	src/appinfo.cpp
	src/aura.cpp
	src/configurationbase.cpp
	src/dependencylocator.cpp
	src/systemdirectories.cpp
	src/userdirectories.cpp
	src/version.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR})

#libaura Packages
find_package(Intl REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Intl::Intl)
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC JsonCpp::JsonCpp)
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)
find_package(maddy CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC maddy::maddy)
find_package(sqlcipher CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC sqlcipher::sqlcipher)
find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SQLiteCpp)
if(LINUX)
	find_package(DBus1 CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME} PUBLIC dbus-1)
endif()

#libaura Install
configure_file(${PROJECT_SOURCE_DIR}/pkg-config/libaura.pc.in libaura.pc @ONLY)
install(TARGETS libaura LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/libaura.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)