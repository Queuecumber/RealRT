#pragma once

#include "Material.hpp"
#include "Ray.hpp"
#include <opencv2/opencv.hpp>

namespace RealRT
{

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
class Shape
{
public:
    Shape(const Material &mat);
    virtual ~Shape(void);
																			
    virtual double Intersect(const Ray &incident, bool &flipNormals) const = 0; //override to return the distance from the origion to the point of intersection
    virtual cv::Vec3d Normal(const cv::Point3d &pt) const = 0;	//override to get the UNIT normal at the given point

    virtual bool IsLight(void) const;  //return true if this shape is a light source

    const Material &Material(void) const;

private:
        const Material &_Material;
};

}
