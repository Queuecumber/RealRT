#pragma once
#include "shape3d.h"


class plane3d :
	public shape3D
{
public:
	plane3d(MATERIAL<> mat = MATERIAL<>(), vector<3,double> normal = vector<3,double>(0.0,1.0,0.0), double d = 5);

	vector<3,double> GetNormal(vector<3,double> pt);

	double HasIntersection(ray &incident,bool &flipnormals);

	bool IsLight(){return false;}

	~plane3d(void);

private:
	vector<3,double> norm;
	double d;
};
