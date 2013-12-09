#include "SphericalLight.hpp"

using namespace RealtRT;

SphericalLight::SphericalLight(const Material &mat, const Vector3D &center, float radius)
    : Sphere(mat, center, radius)
{

}

bool SphericalLight::IsLight(void)
{ 
    return true; 
}

