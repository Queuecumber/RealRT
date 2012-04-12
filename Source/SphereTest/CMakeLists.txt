project(SphereTest)

add_executable(SphereTest SphereTest.cpp)
target_link_libraries(SphereTest RTEngine)

set_target_properties(SphereTest PROPERTIES DEBUG_POSTFIX "d")
