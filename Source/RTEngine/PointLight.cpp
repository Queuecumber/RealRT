#include "PointLight.hpp"

using namespace RealRT;

PointLight::PointLight(std::shared_ptr<const Material> mat, const Vector3D &pos)
    : Light(mat), _Position(pos)
{

}

Vector3D PointLight::Position(void) const
{
    return _Position;
}
