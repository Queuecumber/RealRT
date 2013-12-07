#pragma once

#include "Sphere.hpp"

namespace RealRT
{

    class SphericalLight : public Sphere
    {
    public:
        SphericalLight(MATERIAL<> mat = WHITELIGHT, const cv::Vec3d &center = cv::Vec3d::zeros(3), double radius = 2.5)
            : Sphere(mat, center, radius)
        {

        }

        ~SphericalLight(void) { }

        bool IsLight(void) const { return true; }
    };

}
