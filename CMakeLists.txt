cmake_minimum_required(VERSION 3.5.1)
project(Snake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})


set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

file(GLOB SOURCES "*.cc")
add_executable(snake ${SOURCES})
target_link_libraries(snake ${CURSES_LIBRARIES})
