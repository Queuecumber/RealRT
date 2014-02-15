#include "Material.hpp"

using namespace RealRT;

Material::Material(const Vector3D &color, float reflectance, float refractance, float indexOfRefraction, float diffuseScalar, float specularScalar)
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

float Material::Reflectance(void) const
{
    return _Reflectance;
}

float Material::Refractance(void) const
{
    return _Refractance;
}

float Material::IndexOfRefraction(void) const
{
    return _IndexOfRefraction;
}

float Material::DiffuseScalar(void) const
{
    return _DiffuseScalar;
}

float Material::SpecularScalar(void) const
{
    return _SpecularScalar;
}
