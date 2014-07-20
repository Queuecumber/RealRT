#include "Sphere.hpp"
#include "Vector3D.hpp"
#include "Ray.hpp"
#include <cmath>

using namespace RealRT;

Sphere::Sphere(std::shared_ptr<const Material> mat, const Vector3D &center, float radius)
    : Shape(mat)
    , _Center(center)
    , _Radius(radius)
{

}

float Sphere::Radius(void) const
{
    return _Radius;
}

Vector3D Sphere::Center(void) const
{
    return _Center;
}

float Sphere::Intersect(const Ray &incident, bool &flipNormals) const
{
    Vector3D vec;
    vec = incident.Origin() - _Center;

	//the direction std::vector doted against the position std::vector of the center of the sphere is b
    float b = -(vec * incident.Direction());

	//c is the magnitude squared of the center position std::vector, minus the
	//radius squared
    float c = vec*vec - (_Radius * _Radius);

	//calculate the determinant
	float det = (b * b) - c;

	if(det == 0)
		return b;

	//begin distance calculation
	//
	float retval = 0;
	if(det > 0)
	{
		//calculate both distances
		//
		det = std::sqrt(det);
		float i1 = b - det;
		float i2 = b + det;

		//see if the greater distance value (i2) is positive, if it isnt, then the other distance
		//must be negative and the object is behind the screen
		if(i2 > 0)
		{
			//if i2 is positve and i1 is negative, we are inside the sphere and the closent point of intersection
			//is i2
			if(i1 < 0)
			{
				retval = i2;
				flipNormals = true;
			}
			else
			{
				//otherwise, i1 is the closest point and the entire sphere is in front of the screen
				retval = i1;
				flipNormals = false;
			}
		}
	}

	return retval;
}



Vector3D Sphere::Normal(const Vector3D &pt) const
{
	//the normal to a point on the sphere is the vector along the radius, begining at the center and ending at the
	//given point. Dividing by the radius makes this a unit vector
	Vector3D norm = (pt - _Center).Normalize();

	return norm;
}
