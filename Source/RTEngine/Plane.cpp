#include "Plane.hpp"
#include "Ray.hpp"

using namespace RealRT;

Plane::Plane(const Material &mat, const Vector3D &normal, float d)
    : Shape(mat)
    , _Normal(normal)
    , _D(d)
{

}

Vector3D Plane::Normal(const Vector3D &pt) const
{
    return _Normal;
}

float Plane::Intersect(const Ray &incident, bool &flipNormals) const
{
    flipNormals = false;

    float det = _Normal * incident.Direction();
	if(det != 0)
	{
        float dist = -(_Normal * incident.Origin() + _D) / det;
		if(dist > 0)
			return dist;
	}

	return 0;
}

bool Plane::IsLight(void) const
{
    return false;
}

