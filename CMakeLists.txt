cmake_minimum_required(VERSION 3.10)
project(BallroomOfTheDead CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(ballroom ${SOURCES})
target_include_directories(ballroom PRIVATE ${CMAKE_SOURCE_DIR}/include)

# The game reads and writes the data/ folder in the folder you run it from,
# so run it from the project folder:   ./build/ballroom
