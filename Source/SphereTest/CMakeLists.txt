project(SphereTest)

add_executable(SphereTest SphereTest.cpp)
target_include_directories(SphereTest PUBLIC ${PNG_PNG_INCLUDE_DIR})
target_link_libraries(SphereTest RTEngine ${PNG_LIBRARY} ${PNG_LIBRARY_RELEASE})

set_target_properties(SphereTest PROPERTIES DEBUG_POSTFIX "d")
