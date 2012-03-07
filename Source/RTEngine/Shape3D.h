#pragma once

#include <windows.h>
#include <gmath.h>
#include "Ray.h"

using gmath::vector;


/*
	struct MATERIAL

	proivides basic material information, later support
	for custom refraction algorithms (frequency based)

	PMATERIAL is used for the pointer
*/

template<typename vType = double, typename sType = double>
struct MATERIAL 
{
	gmath::vector<3,vType> color;

	sType reflectance;
	sType refractance;
	sType refIndex;

	sType diffuseScaler;
	sType specularScaler;

	sType (*brdf)(gmath::vector<3,sType> &, gmath::vector<3,sType> &, gmath::vector<3,sType> &, MATERIAL<> &);

	MATERIAL(vector<3,vType> &col,sType refl, sType refr, sType refInd, sType difScaler, sType specScaler,sType (*brd)(gmath::vector<3,sType> &, gmath::vector<3,sType> &, gmath::vector<3,sType> &, MATERIAL<vType,sType> &))
		:color(col),reflectance(refl),refractance(refr),refIndex(refInd),diffuseScaler(difScaler),specularScaler(specScaler)
	{
		brdf = brd;
	}

	MATERIAL(const MATERIAL<vType,sType> &cp)
		:color(cp.color),reflectance(cp.reflectance),refractance(cp.refractance),refIndex(cp.refIndex),diffuseScaler(cp.diffuseScaler),specularScaler(cp.specularScaler)
	{
		brdf = cp.brdf;
	}

	MATERIAL<vType,sType> operator=(const MATERIAL<vType,sType> &cp)
	{
		color = cp.color;
		reflectance = cp.reflectance;
		refractance = cp.refractance;
		refIndex = cp.refIndex;
		diffuseScaler = cp.diffuseScaler;
		specularScaler = cp.specularScaler;
		brdf = cp.brdf;

		return *this;
	}

	MATERIAL(){}
	
};
typedef MATERIAL<> * PMATERIAL;


/*
	class shape3D

	abstract base class for all 3D shapes
	to be used in the ray tracer. 

	Extendable as long as all pure virtual member 
	functions aregiven meaningful implementations, these provide 
	a complete picture of the surface for the ray tracer.

	In addition, the shape3D class manages all
	material related interactions. The material must
	be locked to be used to prevent it from being changed
	while it is being used.
*/
class shape3D
{
public:
	shape3D(MATERIAL<> mat);
	shape3D(PMATERIAL mat);
	virtual ~shape3D(void);
																			
	virtual double HasIntersection(ray &incident, bool &flipnormals)=0;//{return 0;}						//override to return the distance from the origion to the point of intersection
	virtual vector<3,double> GetNormal(vector<3,double> pt)=0;//{return vector<3,double>(0,0,0);}			//override to get the UNIT normal at the given point 
	virtual bool IsLight(){return false;}											//return true if this shape is a light source

	//locks the material and returns a pointer to it, or returns NULL if the material is locked
	PMATERIAL LockMaterial();
	bool UnlockMaterial(PMATERIAL mat);

private:
	MATERIAL<> mat;
	bool locked;
};

double Phong(gmath::vector<3,double> &in, gmath::vector<3,double> &out, gmath::vector<3,double> &unorm, MATERIAL<> &mat);


//some material definitions
#define DIFFGREENOBJECT		MATERIAL<>(vector<3,double>(0.1,0.9,0.1),0.1,0.0,0.0,0.9,0.2,&Phong)
#define DIFFREDOBJECT		MATERIAL<>(vector<3,double>(0.9,0.1,0.1),0.1,0.0,0.0,0.9,0.2,&Phong)
#define DIFFBLUEOBJECT		MATERIAL<>(vector<3,double>(0.1,0.1,0.9),0.1,0.0,0.0,0.9,0.2,&Phong)

#define REFLGREENOBJECT		MATERIAL<>(vector<3,double>(0.1,0.9,0.1),0.9,0.0,0.0,0.1,0.9,&Phong)
#define REFLREDOBJECT		MATERIAL<>(vector<3,double>(0.9,0.1,0.1),0.9,0.0,0.0,0.1,0.9,&Phong)
#define REFLBLUEOBJECT		MATERIAL<>(vector<3,double>(0.1,0.1,0.9),0.9,0.0,0.0,0.1,0.9,&Phong)
#define MIRROR				MATERIAL<>(vector<3,double>(1.0,1.0,1.0),0.9,0.0,0.0,0.1,0.9,&Phong)

#define TRANSGREENOBJECT	MATERIAL<>(vector<3,double>(0.1,0.9,0.2),0.5,0.0,0.0,0.1,0.9,&Phong)
#define TRANSREDOBJECT		MATERIAL<>(vector<3,double>(0.9,0.2,0.1),0.5,0.0,0.0,0.0,1.9,&Phong)
#define TRANSBLUEOBJECT		MATERIAL<>(vector<3,double>(0.2,0.1,0.9),0.5,0.0,0.0,0.1,0.9,&Phong)
#define TRANSPARENTOBJECT	MATERIAL<>(vector<3,double>(1.0,1.0,1.0),0.5,0.9,1.0,0.0,0.9,&Phong)

#define WHITELIGHT			MATERIAL<>(vector<3,double>(1.0,1.0,1.0),0.0,0.0,0.0,1.0,0.0,&Phong)

#define BLANK				MATERIAL<>(vector<3,double>(0.0,0.0,0.0),0.0,0.0,0.0,0.0,0.0,&Phong)