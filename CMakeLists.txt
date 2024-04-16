cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SimpleThreadManager)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


add_library(SimpleThreadManager SHARED
        src/ProcessControl.cpp
        src/ProcessFunction.cpp
        include/IData.hpp
        include/IProcess.hpp
        include/ProcessFunction.hpp
        include/ProcessControl.hpp)

add_executable(TestThreadManager
        TestHarness/TestThreadManager.cpp
        include/ProcessControl.hpp)

add_executable(UnitTestSimpleThreadManager
        UnitTests/UnitTest_ProcessControl.cpp)


target_link_libraries(TestThreadManager SimpleThreadManager)

target_link_libraries(UnitTestSimpleThreadManager gmock_main  pthread SimpleThreadManager)

enable_testing()
include(GoogleTest)
gtest_discover_tests(UnitTestSimpleThreadManager)
