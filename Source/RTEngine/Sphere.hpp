#pragma once
#include "shape3d.h"
#include <gmath.h>

using gmath::vector;


/*
	class sphere <- class shape3D

	provdes the mathematic model of a sphere
*/



class sphere :
	public shape3D
{
public:
	sphere(MATERIAL<> mat = BLANK, vector<3,double> &center = vector<3,double>(), double radius = 2.5);
	~sphere(void);

	double HasIntersection(ray &incident, bool &flipnormals);
	vector<3,double> GetNormal(vector<3,double> pt);

	double GetRadius();
	vector<3,double> &GetCenter();


private:
	vector<3,double> cen;
	double rad;
};


