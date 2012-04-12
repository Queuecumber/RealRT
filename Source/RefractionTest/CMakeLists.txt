project(RefractionTest)

add_executable(RefractionTest RefractionTest.cpp)
target_link_libraries(RefractionTest RTEngine)

set_target_properties(RefractionTest PROPERTIES DEBUG_POSTFIX "d")
