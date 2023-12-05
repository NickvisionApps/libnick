cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SQLITE_HAS_CODEC ON)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

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
	src/keyring/keyring.cpp
	src/keyring/keyringdialogcontroller.cpp
	src/keyring/passwordgenerator.cpp
	src/keyring/store.cpp
	src/keyring/systemcredentials.cpp
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
	SOVERSION ${PROJECT_VERSION})

#libaura Packages
find_package(Intl REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Intl::Intl)
find_package(jsoncpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC JsonCpp::JsonCpp)
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)
find_package(maddy REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC maddy::maddy)
find_package(SQLiteCpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SQLiteCpp)
if(LINUX)
	find_package(libsecret REQUIRED CONFIG)
	target_link_libraries(${PROJECT_NAME} PUBLIC libsecret::libsecret)
endif()

#libaura Install
configure_file(${PROJECT_SOURCE_DIR}/pkg-config/libaura.pc.in libaura.pc @ONLY)
install(TARGETS libaura LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/libaura.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)