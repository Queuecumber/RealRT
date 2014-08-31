#pragma once

#include "Sphere.hpp"

namespace RealRT
{

    class SphericalLight : public Sphere
    {
    public:
        SphericalLight(std::shared_ptr<const Material> mat, const Vector3D &center = {0.0, 0.0, 0.0}, double radius = 2.5);

        bool IsLight(void) const;
    };

}
