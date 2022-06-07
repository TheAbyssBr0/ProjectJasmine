cmake_minimum_required(VERSION 3.14)
project(ProjectJasmine VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip 
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(TaskTimeTest test/TaskTime.test.cpp
			   src/TaskTime.cpp)

target_link_libraries(
    TaskTimeTest
    gtest_main
)

target_include_directories(TaskTimeTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(DayTest test/Day.test.cpp
               src/Day.cpp src/TaskTime.cpp)

target_link_libraries(
    DayTest
    gtest_main
)

target_include_directories(DayTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(TaskTimeTest)
gtest_discover_tests(DayTest)