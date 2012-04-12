#pragma once

#include <opencv2/opencv.hpp>

namespace RealRT
{

    class Material
    {
    public:

        Material(const cv::Scalar &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar);


        virtual double BidirectionReflectanceDistributionFunction(const cv::Vec3d &in, const cv::Vec3d &out, const cv::Vec3d &norm) const = 0;

        cv::Scalar Color(void) const;

        double Reflectance(void) const;
        double Refractance(void) const;

        double IndexOfRefraction(void) const;

        double DiffuseScalar(void) const;
        double SpecularScalar(void) const;

    private:

        cv::Scalar _Color;

        double _Reflectance;
        double _Refractance;
        double _IndexOfRefraction;

        double _DiffuseScalar;
        double _SpecularScalar;
    };

}
