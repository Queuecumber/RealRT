#pragma once

#include "Shape.hpp"
#include "Vector3D.hpp"

namespace RealRT
{

    class Sphere : public Shape
    {
		public:
		    Sphere(std::shared_ptr<const Material> mat, const Vector3D &center = {0.0, 0.0, 0.0}, double radius = 2.5);
		    ~Sphere(void) = default;

		    double Intersect(const Ray &incident) const;
		    Vector3D Normal(const Vector3D &pt) const;

		    double Radius(void) const;
		    Vector3D Center(void) const;

		private:
		    Vector3D _Center;
		    double _Radius;
    };

}
