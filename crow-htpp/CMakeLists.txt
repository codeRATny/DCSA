cmake_minimum_required(VERSION 3.0)
project(crow-http)

set(TARGET_NAME "crow-http")

set(CROW_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/contrib/crow)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -O3")

find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wempty-body -Wredundant-move -O2")

set(SOURCE_DIR_HEADER "")
execute_process(
    COMMAND find src -type d
    OUTPUT_VARIABLE SOURCE_DIR_HEADER
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "\n" ";" SOURCE_DIR_HEADER ${SOURCE_DIR_HEADER})
    
include_directories(${SOURCE_DIR_HEADER})
include_directories(${CROW_INCLUDE})
    
add_executable(${TARGET_NAME} ${SOURCE})
target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES})