set(HV_ROOT_DIR "${PROJECT_SOURCE_DIR}/contrib/libhv")
if(NOT EXISTS "${HV_ROOT_DIR}/CMakeLists.txt")
  message(FATAL_ERROR " submodule libhv is missing. To fix try run: \n git submodule update --init --recursive")
endif()

set(BUILD_SHARED_LIBS "FALSE")
add_subdirectory("${HV_ROOT_DIR}" "${PROJECT_BINARY_DIR}/contrib/libhv")
add_library(hv_library INTERFACE)

target_link_libraries(hv_library INTERFACE
  hv_static
)

target_include_directories(hv_library 
  PRIVATE INTERFACE
    "$<BUILD_INTERFACE:${HV_ROOT_DIR}/include}>"
  PUBLIC INTERFACE
    "$<BUILD_INTERFACE:${HV_ROOT_DIR}/include}>")

get_target_property(HV_INCLUDE_DIR hv_static INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories (hv_library SYSTEM BEFORE INTERFACE ${HV_INCLUDE_DIR})