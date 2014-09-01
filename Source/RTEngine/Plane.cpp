#include "Plane.hpp"
#include "Ray.hpp"

using namespace RealRT;

Plane::Plane(std::shared_ptr<const Material> mat, const Vector3D &normal, double d)
    : Shape(mat)
    , _Normal(normal)
    , _D(d)
{

}

Vector3D Plane::Normal(const Vector3D &pt) const
{
    return _Normal;
}

double Plane::Intersect(const Ray &incident) const
{
    // Project the incident ray direction into the normal of the plane
    double det = _Normal * incident.Direction();

    // If this projection is 0, then the incident direction is parallel to the plane and there is no intersection
	if(det != 0)
	{
        double dist = -(_Normal * incident.Origin() + _D) / det;
		return dist;
	}

	return 0;
}

bool Plane::IsLight(void) const
{
    return false;
}
