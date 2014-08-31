#include "Light.hpp"
#include "Material.hpp"

using namespace RealRT;

Light::Light(std::shared_ptr<const Material> mat)
    : _Material(mat)
{

}

const std::shared_ptr<const Material> Light::LightMaterial(void) const
{
    return _Material;
}
