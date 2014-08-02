#pragma once
#include <memory>

namespace RealRT
{
	class Material;
	class Ray;
	class Vector3D;

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
            Shape(std::shared_ptr<const Material> mat);
			virtual ~Shape(void) = default;

			virtual double Intersect(const Ray &incident, bool &flipNormals) const = 0; //override to return the distance from the origin to the point of intersection
			virtual Vector3D Normal(const Vector3D &pt) const = 0;	//override to get the UNIT normal at the given point

			virtual bool IsLight(void) const;  //return true if this shape is a light source

            const std::shared_ptr<const Material> SurfaceMaterial(void) const;

		private:
                std::shared_ptr<const Material> _Material;
	};
}
