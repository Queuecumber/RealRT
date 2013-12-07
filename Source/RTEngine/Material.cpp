#include "Material.hpp"

using namespace RealRT;

Material::Material(const cv::Scalar &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar)
    : _Color(color)
    , _Reflectance(reflectance)
    , _Refractance(refractance)
    , _IndexOfRefraction(indexOfRefraction)
    , _DiffuseScalar(diffuseScalar)
    , _SpecularScalar(specularScalar)
{

}

Material::Material(const Material &cp)
    : _Color(cp.Color())
    , _Reflectance(cp.Reflectance())
    , _Refractance(cp.Refractance())
    , _IndexOfRefraction(cp.IndexOfRefraction())
    , _DiffuseScalar(cp.DiffuseScalar())
    , _SpecularScalar(cp.SpecularScalar())
{

}

cv::Scalar Material::Color(void)
{
    return _Color;
}

double Material::Reflectance(void)
{
    return _Reflectance;
}

double Material::Refractance(void)
{
    return _Refractance;
}

double Material::IndexOfRefraction(void)
{
    return _IndexOfRefraction;
}

double Material::DiffuseScalar(void)
{
    return _DiffuseScalar;
}

double Material::SpecularScalar(void)
{
    return _SpecularScalar;
}
