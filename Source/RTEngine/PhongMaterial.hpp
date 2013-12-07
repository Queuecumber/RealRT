#pragma once

#include "Material.hpp"

namespace RealRT
{

    class PhongMaterial : public Material
    {
    public:

        PhongMaterial(const cv::Scalar &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar);

        double BidirectionReflectanceDistributionFunction(const cv::Vec3d &in, const cv::Vec3d &out, const cv::Vec3d &norm) const;
    };

    //some material definitions
    const PhongMaterial DiffuseRed(cv::Scalar(0.9,0.1,0.1),0.1,0.0,0.0,0.9,0.2);
    const PhongMaterial DiffuseGreen(cv::Scalar(0.1,0.9,0.1),0.1,0.0,0.0,0.9,0.2);
    const PhongMaterial DiffuseBlue(cv::Scalar(0.1,0.1,0.9),0.1,0.0,0.0,0.9,0.2);

    const PhongMaterial ReflectiveRed(cv::Scalar(0.9,0.1,0.1),0.9,0.0,0.0,0.1,0.9);
    const PhongMaterial ReflectiveGreen(cv::Scalar(0.1,0.9,0.1),0.9,0.0,0.0,0.1,0.9);
    const PhongMaterial ReflectiveBlue(cv::Scalar(0.1,0.1,0.9),0.9,0.0,0.0,0.1,0.9);

    const PhongMaterial Mirror(cv::Scalar(1.0,1.0,1.0),0.9,0.0,0.0,0.1,0.9);

    const PhongMaterial TranslucentGreen(cv::Scalar(0.1,0.9,0.2),0.5,0.0,0.0,0.1,0.9);
    const PhongMaterial TranslucentRed(cv::Scalar(0.9,0.2,0.1),0.5,0.0,0.0,0.0,1.9);
    const PhongMaterial TranslucentBlue(cv::Scalar(0.2,0.1,0.9),0.5,0.0,0.0,0.1,0.9);

    const PhongMaterial Transparent(cv::Scalar(1.0,1.0,1.0),0.5,0.9,1.0,0.0,0.9);

    const PhongMaterial WhiteLight(cv::Scalar(1.0,1.0,1.0),0.0,0.0,0.0,1.0,0.0);

    const PhongMaterial Blank(cv::Scalar(0.0,0.0,0.0),0.0,0.0,0.0,0.0,0.0);

}
