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


add_executable(ScheduleTest test/Schedule.test.cpp
               src/Schedule.cpp src/Day.cpp src/TaskTime.cpp)

target_link_libraries(
    ScheduleTest
    gtest_main
)

target_include_directories(ScheduleTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(CheckTest test/Check.test.cpp
               src/Check.cpp)

target_link_libraries(
    CheckTest
    gtest_main
)

target_include_directories(CheckTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(JournalTest test/Journal.test.cpp
               src/Journal.cpp src/Check.cpp src/Schedule.cpp src/Day.cpp src/TaskTime.cpp)

target_link_libraries(
    JournalTest
    gtest_main
)

target_include_directories(JournalTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(TimeTest test/Time.test.cpp
               src/Time.cpp)

target_link_libraries(
    TimeTest
    gtest_main
)

target_include_directories(TimeTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(TaskTimeTest)
gtest_discover_tests(DayTest)
gtest_discover_tests(ScheduleTest)
gtest_discover_tests(CheckTest)
gtest_discover_tests(JournalTest)
gtest_discover_tests(TimeTest)