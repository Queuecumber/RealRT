#pragma once

#include "Material.hpp"
#include <memory>

namespace RealRT
{

    class PhongMaterial : public Material
    {
        public:

            PhongMaterial(const Vector3D &color, float reflectance, float refractance, float indexOfRefraction, float diffuseScalar, float specularScalar);

            float BidirectionReflectanceDistributionFunction(const Vector3D &in, const Vector3D &out, const Vector3D &norm) const;
    };

    //some material definitions
    const std::shared_ptr<Material> DiffuseRed = std::make_shared<PhongMaterial>(Vector3D(0.9f,0.1f,0.1f),0.1f,0.0f,0.0f,0.9f,0.2f);
    const std::shared_ptr<Material> DiffuseGreen = std::make_shared<PhongMaterial>(Vector3D(0.1f,0.9f,0.1f),0.1f,0.0f,0.0f,0.9f,0.2f);
    const std::shared_ptr<Material> DiffuseBlue = std::make_shared<PhongMaterial>(Vector3D(0.1f,0.1f,0.9f),0.1f,0.0f,0.0f,0.9f,0.2f);

    const std::shared_ptr<Material> ReflectiveRed = std::make_shared<PhongMaterial>(Vector3D(0.9f,0.1f,0.1f),0.9f,0.0f,0.0f,0.1f,0.9f);
    const std::shared_ptr<Material> ReflectiveGreen = std::make_shared<PhongMaterial>(Vector3D(0.1f,0.9f,0.1f),0.9f,0.0f,0.0f,0.1f,0.9f);
    const std::shared_ptr<Material> ReflectiveBlue = std::make_shared<PhongMaterial>(Vector3D(0.1f,0.1f,0.9f),0.9f,0.0f,0.0f,0.1f,0.9f);

    const std::shared_ptr<Material> Mirror = std::make_shared<PhongMaterial>(Vector3D(1.0f,1.0f,1.0f),0.9f,0.0f,0.0f,0.1f,0.9f);

    const std::shared_ptr<Material> TranslucentGreen = std::make_shared<PhongMaterial>(Vector3D(0.1f,0.9f,0.2f),0.5f,0.0f,0.0f,0.1f,0.9f);
    const std::shared_ptr<Material> TranslucentRed = std::make_shared<PhongMaterial>(Vector3D(0.9f,0.2f,0.1f),0.5f,0.0f,0.0f,0.0f,1.9f);
    const std::shared_ptr<Material> TranslucentBlue = std::make_shared<PhongMaterial>(Vector3D(0.2f,0.1f,0.9f),0.5f,0.0f,0.0f,0.1f,0.9f);

    const std::shared_ptr<Material> Transparent = std::make_shared<PhongMaterial>(Vector3D(1.0f,1.0f,1.0f),0.5f,0.9f,1.0f,0.0f,0.9f);

    const std::shared_ptr<Material> WhiteLight = std::make_shared<PhongMaterial>(Vector3D(1.0f,1.0f,1.0f),0.0f,0.0f,0.0f,1.0f,0.0f);

    const std::shared_ptr<Material> Blank = std::make_shared<PhongMaterial>(Vector3D(0.0f,0.0f,0.0f),0.0f,0.0f,0.0f,0.0f,0.0f);

}
