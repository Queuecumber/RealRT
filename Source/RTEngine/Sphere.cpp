#include "Sphere.hpp"

using namespace RealRT;

Sphere::Sphere(MATERIAL<> mat, const cv::Vec3d &center, double radius)
    : Shape(mat)
    , _Center(center)
    , _Radius(radius)
{

}

Sphere::~Sphere(void)
{

}

double Sphere::Radius(void) const
{
    return _Radius;
}

cv::Point3d Sphere::Center(void) const
{
    return _Center;
}

double Sphere::Intersect(const Ray &incident, bool &flipNormals) const
{
    cv::Vec3d vec;
    vec = incident.Origin() - _Center;

	//the direction std::vector doted against the position std::vector of the center of the sphere is b 
    double b = -(vec * incident.Direction());

	//c is the magnitude squared of the center position std::vector, minus the
	//radius squared
    double c = vec*vec - (_Radius * _Radius);

	//calculate the determinant
	double det = (b * b) - c;

	if(det == 0)
		return b; 

	//begin distance calculation
	//
	double retval = 0;
	if(det > 0)
	{
		//calculate both distances
		//
		det = sqrt(det);
		double i1 = b - det;
		double i2 = b + det;

		//see if the greater distance value (i2) is positive, if it isnt, then the other distance
		//must be negative and the object is behind the screen
		if(i2 > 0)
		{
			//if i2 is positve and i1 is negative, we are inside the sphere and the closent point of intersection 
			//is i2
			if(i1 < 0)
			{
				retval = i2;
				flipnormals = true;
			}else{
				//otherwise, i1 is the closest point and the entire sphere is in front of the screen
				retval = i1;
				flipnormals = false;
			}
		}
	}
	return retval;
}



cv::Vec3d Sphere::Normal(const cv::Point3d &pt) const
{
	//the normal to a point on the sphere is the vector along the radius, begining at the center and ending at the 
	//given point. Dividing by the radius makes this a unit vector
	vector<3,double> unorm;

	unorm = (pt - cen) * rad;
	//unorm->Normalize();

	return unorm;
}
