#pragma once

#include "Sphere.hpp"

namespace RealRT
{

    class SphericalLight : public Sphere
    {
    public:
        SphericalLight(std::shared_ptr<const Material> mat, const Vector3D &center = {0.f, 0.f, 0.f}, float radius = 2.5f);

        bool IsLight(void) const;
    };

}
