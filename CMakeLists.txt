cmake_minimum_required(VERSION 3.27)
project(SimpleThreadManager)

set(CMAKE_CXX_STANDARD 23)

add_library(SimpleThreadManager SHARED
        src/ProcessControl.cpp
        src/ProcessFunction.cpp
        include/IData.hpp
        include/IProcess.hpp
        include/ProcessFunction.hpp
        include/ProcessControl.hpp)

add_executable(TestThreadManager
        src/TestThreadManager.cpp
        include/ProcessControl.hpp)


target_link_libraries(TestThreadManager SimpleThreadManager)