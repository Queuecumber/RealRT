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
    Vector3D.hpp
    RecursiveTraceStrategy.hpp
    IterativeTraceStrategy.hpp
)

set( RTEngine_SOURCES
    Plane.cpp
    Ray.cpp
    RTEngine.cpp
    Shape.cpp
    Sphere.cpp
    SphericalLight.cpp
    Material.cpp
    PhongMaterial.cpp
    Vector3D.cpp
    RecursiveTraceStrategy.cpp
    IterativeTraceStrategy.cpp
)

add_library(RTEngine ${RTEngine_HEADERS} ${RTEngine_SOURCES})

set_target_properties(RTEngine PROPERTIES DEBUG_POSTFIX "d")
