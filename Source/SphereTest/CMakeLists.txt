project(SphereTest)

add_executable(SphereTest SphereTest.cpp)
target_link_libraries(SphereTest RTEngine ${PNG_LIBRARY_RELEASE})

set_target_properties(SphereTest PROPERTIES DEBUG_POSTFIX "d")
