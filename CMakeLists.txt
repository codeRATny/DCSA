cmake_minimum_required(VERSION 3.0)
project(http-server)
set(CMAKE_CXX_STANDARD 20)

set(TARGET_NAME "http-server")

set(LIBHV_INCLUDE ${PROJECT_BINARY_DIR}/contrib/libhv/include/hv)

find_package(Boost COMPONENTS thread REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/contrib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wempty-body -Wredundant-move -O2")

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety")
endif()

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

set(SOURCE_DIR_HEADER "")
execute_process(
    COMMAND find src -type d
    OUTPUT_VARIABLE SOURCE_DIR_HEADER
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "\n" ";" SOURCE_DIR_HEADER ${SOURCE_DIR_HEADER})

include_directories(${SOURCE_DIR_HEADER})
include_directories(${LIBHV_INCLUDE})

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} hv_static)
target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})