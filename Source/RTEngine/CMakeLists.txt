project(RTEngine)

set( RTEngine_HEADERS

    gmath.h
    light.h
    plane3d.h
    Ray.h
    RTEngine.h
    Shape3D.h
    Sphere.h
    SphereicalLight.h
    stdafx.h
    Vector3D.h
)

set( RTEngine_SOURCES 

    light.cpp
    plane3d.cpp
    ray.cpp
    RTEngine.cpp
    shape3D.cpp
    sphere.cpp
    SphereicalLight.cpp
    stdafx.cpp
    vector3D.cpp
) 

add_library(RTEngine SHARED ${RTEngine_HEADERS} ${RTEngine_SOURCES})

