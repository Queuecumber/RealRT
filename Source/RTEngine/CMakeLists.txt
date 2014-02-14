project(RTEngine)

set( RTEngine_HEADERS
    Plane.hpp
    Ray.hpp
    RTEngine.hpp
    Shape.hpp
    Sphere.hpp
    SphericalLight.hpp
    Material.hpp
    PhongMaterial.hpp
)

set( RTEngine_SOURCES 
    Plane.cpp
    Ray.cpp
    RTEngine.cpp
    Shape.cpp
    Sphere.cpp
    Material.cpp
    PhongMaterial.cpp
) 

add_library(RTEngine SHARED ${RTEngine_HEADERS} ${RTEngine_SOURCES})

set_target_properties(RTEngine PROPERTIES DEBUG_POSTFIX "d")
