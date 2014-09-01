#pragma once

#include "Material.hpp"
#include <memory>

namespace RealRT
{

    class PhongMaterial : public Material
    {
        public:

            PhongMaterial(const Vector3D &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar);

            double Brdf(const Vector3D &in, const Vector3D &out, const Vector3D &norm) const;
    };

    //some material definitions
    const std::shared_ptr<Material> DiffuseRed = std::make_shared<PhongMaterial>(Vector3D(0.9,0.1,0.1),0.1,0.0,0.0,0.9,0.2);
    const std::shared_ptr<Material> DiffuseGreen = std::make_shared<PhongMaterial>(Vector3D(0.1,0.9,0.1),0.1,0.0,0.0,0.9,0.2);
    const std::shared_ptr<Material> DiffuseBlue = std::make_shared<PhongMaterial>(Vector3D(0.1,0.1,0.9),0.1,0.0,0.0,0.9,0.2);

    const std::shared_ptr<Material> ReflectiveRed = std::make_shared<PhongMaterial>(Vector3D(0.9,0.1,0.1),0.9,0.0,0.0,0.1,0.9);
    const std::shared_ptr<Material> ReflectiveGreen = std::make_shared<PhongMaterial>(Vector3D(0.1,0.9,0.1),0.9,0.0,0.0,0.1,0.9);
    const std::shared_ptr<Material> ReflectiveBlue = std::make_shared<PhongMaterial>(Vector3D(0.1,0.1,0.9),0.9,0.0,0.0,0.1,0.9);

    const std::shared_ptr<Material> Mirror = std::make_shared<PhongMaterial>(Vector3D(1.0,1.0,1.0),0.9,0.0,0.0,0.1,0.9);

    const std::shared_ptr<Material> TranslucentGreen = std::make_shared<PhongMaterial>(Vector3D(0.1,0.9,0.2),0.4,0.9,1.3,0.1,0.9);
    const std::shared_ptr<Material> TranslucentRed = std::make_shared<PhongMaterial>(Vector3D(0.9,0.2,0.1),0.4,0.9,1.3,0.1,0.9);
    const std::shared_ptr<Material> TranslucentBlue = std::make_shared<PhongMaterial>(Vector3D(0.2,0.1,0.9),0.4,0.9,1.3,0.1,0.9);

    const std::shared_ptr<Material> Transparent = std::make_shared<PhongMaterial>(Vector3D(1.0,1.0,1.0),0.1,0.9,1.0,0.0,0.9);

    const std::shared_ptr<Material> Glass = std::make_shared<PhongMaterial>(Vector3D(1.0,1.0,1.0),0.1,0.9,1.5,0.0,0.9);
    const std::shared_ptr<Material> Water = std::make_shared<PhongMaterial>(Vector3D(1.0,1.0,1.0),0.1,0.9,1.3,0.0,0.9);

    const std::shared_ptr<Material> WhiteLight = std::make_shared<PhongMaterial>(Vector3D(1.0,1.0,1.0),0.0,0.0,0.0,1.0,0.0);

    const std::shared_ptr<Material> Blank = std::make_shared<PhongMaterial>(Vector3D(0.0,0.0,0.0),0.0,0.0,0.0,0.0,0.0);

}
