#include "PhongMaterial.hpp"
#include <cmath>

using namespace RealRT;

PhongMaterial::PhongMaterial(const Vector3D &color, float reflectance, float refractance, float indexOfRefraction, float diffuseScalar, float specularScalar)
    : Material(color, reflectance, refractance, indexOfRefraction, diffuseScalar, specularScalar)
{

}

float PhongMaterial::BidirectionReflectanceDistributionFunction(const Vector3D &in, const Vector3D &out, const Vector3D &norm)  const
{
    float ratio = 0.f;

    //take the dot product of the unit normal against the vector to the light. Note that
    //since both are unit vectors, this value is the cosine of the angle between them
    float dot = norm * out;

    //use only positive cosine values to shade only the parts facing the light source
    //
    if(dot > 0)
    {
        if(DiffuseScalar() > 0)
        {
            ratio += dot * DiffuseScalar();
        }
    }

    Vector3D reflectionDir;
    reflectionDir = out - 2.f * (out * norm) * norm;
    dot = reflectionDir * in;

    if(dot > 0)
    {
        if(SpecularScalar() > 0)
        {
            ratio += pow(dot,50) * SpecularScalar();
        }
    }

    return ratio;
}
