#include "Material.hpp"

using namespace RealRT;

Material::Material(const Vector3D &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar)
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

Vector3D Material::Color(void) const
{
    return _Color;
}

double Material::Reflectance(void) const
{
    return _Reflectance;
}

double Material::Refractance(void) const
{
    return _Refractance;
}

double Material::IndexOfRefraction(void) const
{
    return _IndexOfRefraction;
}

double Material::DiffuseScalar(void) const
{
    return _DiffuseScalar;
}

double Material::SpecularScalar(void) const
{
    return _SpecularScalar;
}
