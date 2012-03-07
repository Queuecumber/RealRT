#include "shape3D.h"




double Phong(gmath::vector<3,double> &in, gmath::vector<3,double> &out, gmath::vector<3,double> &unorm, MATERIAL<> &mat)
{
	double ratio = 0.0;

	//take the dot product of the unit normal against the vector to the light. Note that
	//since both are unit vectors, this value is the cosine of the angle between them
	double dot = unorm * out;

	//use only positive cosine values to shade only the parts facing the light source
	//
	if(dot > 0)
	{
		if(mat.diffuseScaler > 0)
		{
			ratio += dot * mat.diffuseScaler;
		}
	}

	vector<3,double> reflectionDir;
	reflectionDir = out - 2.0 * (out * unorm) * unorm;
	reflectionDir = reflectionDir.normalize();
	dot = reflectionDir * in;

	if(dot > 0)
	{
		if(mat.specularScaler > 0)
		{
			ratio += pow(dot,50) * mat.specularScaler;
		}
	} 

	return ratio;
}

shape3D::shape3D(MATERIAL<> mat)
{
	this->mat = mat;
	locked = false;
}

shape3D::shape3D(PMATERIAL mat)
{
	this->mat = *mat;

	locked = false;
}

shape3D::~shape3D(void){}

PMATERIAL shape3D::LockMaterial()
{
	if(!locked)
	{
		locked=true; 
		return &mat;
	} 
	
	return &mat;
}

bool shape3D::UnlockMaterial(PMATERIAL mat)
{
	if(!locked)
		return true;
	
	if(mat == &(this->mat))
	{
		mat = NULL;
		locked = false; 
		return true;
	}else{
		return false;
	} 
}

