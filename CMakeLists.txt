CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(task-runner)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fext-numeric-literals \
    -Wall -Wextra -Weffc++ -pedantic -pthread")

SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES( ../task-runner )

ADD_EXECUTABLE(${PROJECT_NAME} test/TaskRunnerUnittest.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} boost_system)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
