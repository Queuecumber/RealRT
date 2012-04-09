/*
	Ray.h

	deffinition of the ray class used in the 
	ray tracing algorithm
*/
#pragma once

#include <gmath.h>
using gmath::vector;

class ray
{
public:
	ray(void);
	ray(vector<3,double> &start, vector<3,double> &direction);
	ray(vector<3,double> *start, vector<3,double> *direction);
	ray(ray &cp);

	vector<3,double> GetStart();
	vector<3,double> GetDirection();

	~ray(void);

private:
	vector<3,double> pos;
	vector<3,double> dir;
};
