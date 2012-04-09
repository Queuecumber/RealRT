project(RTEngine)

set( RTEngine_HEADERS
    Light.hpp
    Plane.hpp
    Ray.hpp
    RTEngine.hpp
    Shape.hpp
    Sphere.hpp
    SphereicalLight.hpp
)

set( RTEngine_SOURCES 
    Light.cpp
    Plane.cpp
    Ray.cpp
    RTEngine.cpp
    Shape.cpp
    Sphere.cpp
    SphereicalLight.cpp
) 

add_library(RTEngine SHARED ${RTEngine_HEADERS} ${RTEngine_SOURCES})

