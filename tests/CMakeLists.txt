project("libaura_test")

enable_testing()

add_executable(${PROJECT_NAME}
	eventtests.cpp
	main.cpp
	networktests.cpp
	stringtests.cpp
	versiontests.cpp
	webtests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE libaura-static)
find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})