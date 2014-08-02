#include "SphericalLight.hpp"

using namespace RealRT;

SphericalLight::SphericalLight(std::shared_ptr<const Material> mat, const Vector3D &center, double radius)
    : Sphere(mat, center, radius)
{

}

bool SphericalLight::IsLight(void) const
{
    return true;
}
