#include "PhongMaterial.hpp"

using namespace RealRT;

PhongMaterial::PhongMaterial(const cv::Scalar &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar)
    : Material(color, reflectance, refractance, indexOfRefraction, diffuseScalar, specularScalar)
{

}

PhongMaterial::BidirectionReflectanceDistributionFunction(const cv::Vec3d &in, const cv::Vec3d &out, const cv::Vec3d &norm)
{
    double ratio = 0.0;

    //take the dot product of the unit normal against the vector to the light. Note that
    //since both are unit vectors, this value is the cosine of the angle between them
    double dot = norm * out;

    //use only positive cosine values to shade only the parts facing the light source
    //
    if(dot > 0)
    {
        if(DiffuseScaler() > 0)
        {
            ratio += dot * DiffuseScaler();
        }
    }

    cv::Vec3d reflectionDir;
    reflectionDir = out - 2.0 * (out * norm) * norm;
    dot = reflectionDir * in;

    if(dot > 0)
    {
        if(SpecularScaler() > 0)
        {
            ratio += pow(dot,50) * SpecularScaler();
        }
    }

    return ratio;
}
