#include "plane3d.h"


plane3d::plane3d(MATERIAL<> mat, vector<3,double> normal, double d)
: shape3D(mat)
{
	norm = normal.normalize();
	this->d = d;
}

vector<3,double> plane3d::GetNormal(vector<3,double> pt)
{
	return norm;
}

double plane3d::HasIntersection(ray &incident, bool &flipnormals)
{
	flipnormals = false;
	double det = norm * incident.GetDirection();
	if(det != 0)
	{
		double dist = -(norm * incident.GetStart() + d) / det;
		if(dist > 0)
			return dist;
	}

	return 0;
}

plane3d::~plane3d(void)
{
}

