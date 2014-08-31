#pragma once

#include "Light.hpp"

namespace RealRT
{

    class PointLight : public Light
    {
    public:
        PointLight(std::shared_ptr<const Material> mat, const Vector3D &pos = {0.0, 0.0, 0.0});

        Vector3D Position(void) const;

    private:
        Vector3D _Position;

    };

}
