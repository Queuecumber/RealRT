#include "Ray.hpp"

using namespace RealRT;

Ray::Ray(const Vector3D &start, const Vector3D &direction)
    : _Origin(start)
    , _Direction(direction.Normalize())
{

}

Ray::Ray(const Ray &cp)
    : _Origin(cp.Origin())
    , _Direction(cp.Direction())
{

}

Vector3D Ray::Origin(void) const
{
    return _Origin;
}

Vector3D Ray::Direction(void) const
{
    return _Direction;
}

