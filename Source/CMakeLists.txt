cmake_minimum_required(VERSION 2.8.4)
project(RealRT)

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

if(${LIB64} STREQUAL "TRUE")
    set(LIBSUFFIX "64")
else()
    set(LIBSUFFIX "")
endif()

set(DEBUG_POSTFIX "d")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BIN_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BIN_DIR}/lib${LIBSUFFIX})

add_subdirectory(RTEngine)
add_subdirectory(SphereTest)
add_subdirectory(RefractionTest)
