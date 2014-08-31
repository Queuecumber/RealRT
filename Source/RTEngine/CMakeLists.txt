project(RTEngine)

set( RTEngine_HEADERS
    Plane.hpp
    Ray.hpp
    RTEngine.hpp
    Shape.hpp
    Sphere.hpp
    PointLight.hpp
    Material.hpp
    PhongMaterial.hpp
    Vector3D.hpp
    RecursiveTraceStrategy.hpp
    IterativeTraceStrategy.hpp
    SyncRenderStrategy.hpp
    AsyncRenderStrategy.hpp
    Light.hpp
)

set( RTEngine_SOURCES
    Plane.cpp
    Ray.cpp
    RTEngine.cpp
    Shape.cpp
    Sphere.cpp
    PointLight.cpp
    Material.cpp
    PhongMaterial.cpp
    Vector3D.cpp
    RecursiveTraceStrategy.cpp
    IterativeTraceStrategy.cpp
    SyncRenderStrategy.cpp
    Light.cpp
)

add_library(RTEngine ${RTEngine_HEADERS} ${RTEngine_SOURCES})

set_target_properties(RTEngine PROPERTIES DEBUG_POSTFIX "d")
