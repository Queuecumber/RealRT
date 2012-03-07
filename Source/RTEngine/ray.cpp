#include "Ray.h"

ray::ray(void)
: pos(vector<3,double>(0,0,0)),dir(vector<3,double>(0,0,1)){}

ray::ray(vector<3,double> &start, vector<3,double> &direction)
: pos(start),dir(direction.normalize()){}

ray::ray(vector<3,double> *start, vector<3,double> *direction)
: pos(*start),dir(direction->normalize()){}

ray::ray(ray &cp)
: pos( cp.GetStart() ), dir(cp.GetDirection() ){}

vector<3,double> ray::GetStart()
{
	return pos;
}

vector<3,double> ray::GetDirection()
{
	return dir;
}

ray::~ray(){}
