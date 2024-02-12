cmake_minimum_required(VERSION 3.0)
project(nlohmann-json)

set(JSON_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/contrib/nlohmann-json)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wempty-body -Wredundant-move")

include_directories(${JSON_INCLUDE})
    
add_executable(arrays ${CMAKE_CURRENT_SOURCE_DIR}/src/arrays.cpp)
add_executable(deserialization ${CMAKE_CURRENT_SOURCE_DIR}/src/deserialization.cpp)
add_executable(handle_errors ${CMAKE_CURRENT_SOURCE_DIR}/src/handle_errors.cpp)
add_executable(serialization ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization.cpp)