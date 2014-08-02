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

double Plane::Intersect(const Ray &incident, bool &flipNormals) const
{
    flipNormals = false;

    double det = _Normal * incident.Direction();
	if(det != 0)
	{
        double dist = -(_Normal * incident.Origin() + _D) / det;
		if(dist > 0)
			return dist;
	}

	return 0;
}

bool Plane::IsLight(void) const
{
    return false;
}
