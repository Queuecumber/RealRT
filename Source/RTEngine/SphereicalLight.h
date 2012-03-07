#pragma once
#include "sphere.h"
#include "light.h"

class SphericalLight :
	public sphere, public light
{
public:
	SphericalLight(MATERIAL<> mat = WHITELIGHT, vector<3,double> &center = vector<3,double>(), double radius = 2.5)
		:sphere(mat,center,radius),light(){}
	~SphericalLight(void){}

	bool IsLight(){return light::IsLight();}
};
