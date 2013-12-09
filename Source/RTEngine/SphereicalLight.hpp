#pragma once

#include "Sphere.hpp"

namespace RealRT
{

    class SphericalLight : public Sphere
    {
    public:
        SphericalLight(const Material &mat, const Vector3D &center = {0.f, 0.f, 0.f}, float radius = 2.5f)
            : Sphere(mat, center, radius)
        {

        }

        ~SphericalLight(void) = default;

        bool IsLight(void) const { return true; }
    };

}
