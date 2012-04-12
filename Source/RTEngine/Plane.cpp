#include "Plane.hpp"

using namespace RealRT;

Plane::Plane(MATERIAL<> mat, const cv::Vec3d &normal, double d)
    : shape3D(mat)
    , _Normal(normal)
    , _D(d)
{

}

cv::Vec3d Plane::Normal(const cv::Point3d &pt) const
{
    return _Normal;
}

double Plane::Intersect(const Ray &incident, bool &flipNormals) const
{
	flipnormals = false;

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

Plane::~Plane(void)
{

}


