#include "Ray.hpp"

using namespace RealRT;

Ray::Ray(void)
    : _Origin(0,0,0)
    , _Direction(0,0,1)
{

}

Ray::Ray(const cv::Point3d &start, const cv::Vec3d &direction)
    : _Origin(start)
    , _Direction(direction.normalize())
{

}

Ray::Ray(const Ray &cp)
    : _Origin(cp.Origin())
    , _Direction(cp.Direction())
{

}

cv::Point3d Ray::Origin(void) const
{
    return _Origin;
}

cv::Vec3d Ray::Direction(void) const
{
    return _Direction;
}

Ray::~Ray(void)
{

}
