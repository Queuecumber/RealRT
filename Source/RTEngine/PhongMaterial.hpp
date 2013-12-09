#pragma once

#include "Material.hpp"

namespace RealRT
{

    class PhongMaterial : public Material
    {
        public:

            PhongMaterial(const Vector3D &color, float reflectance, float refractance, float indexOfRefraction, float diffuseScalar, float specularScalar);

            float BidirectionReflectanceDistributionFunction(const Vector3D &in, const Vector3D &out, const Vector3D &norm) const;
    };

    //some material definitions
    const PhongMaterial DiffuseRed(Vector3D(0.9f,0.1f,0.1f),0.1f,0.0f,0.0f,0.9f,0.2f);
    const PhongMaterial DiffuseGreen(Vector3D(0.1f,0.9f,0.1f),0.1f,0.0f,0.0f,0.9f,0.2f);
    const PhongMaterial DiffuseBlue(Vector3D(0.1f,0.1f,0.9f),0.1f,0.0f,0.0f,0.9f,0.2f);

    const PhongMaterial ReflectiveRed(Vector3D(0.9f,0.1f,0.1f),0.9f,0.0f,0.0f,0.1f,0.9f);
    const PhongMaterial ReflectiveGreen(Vector3D(0.1f,0.9f,0.1f),0.9f,0.0f,0.0f,0.1f,0.9f);
    const PhongMaterial ReflectiveBlue(Vector3D(0.1f,0.1f,0.9f),0.9f,0.0f,0.0f,0.1f,0.9f);

    const PhongMaterial Mirror(Vector3D(1.0f,1.0f,1.0f),0.9f,0.0f,0.0f,0.1f,0.9f);

    const PhongMaterial TranslucentGreen(Vector3D(0.1f,0.9f,0.2f),0.5f,0.0f,0.0f,0.1f,0.9f);
    const PhongMaterial TranslucentRed(Vector3D(0.9f,0.2f,0.1f),0.5f,0.0f,0.0f,0.0f,1.9f);
    const PhongMaterial TranslucentBlue(Vector3D(0.2f,0.1f,0.9f),0.5f,0.0f,0.0f,0.1f,0.9f);

    const PhongMaterial Transparent(Vector3D(1.0f,1.0f,1.0f),0.5f,0.9f,1.0f,0.0f,0.9f);

    const PhongMaterial WhiteLight(Vector3D(1.0f,1.0f,1.0f),0.0f,0.0f,0.0f,1.0f,0.0f);

    const PhongMaterial Blank(Vector3D(0.0f,0.0f,0.0f),0.0f,0.0f,0.0f,0.0f,0.0f);

}
