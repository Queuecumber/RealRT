#pragma once

#include "Shape.hpp"

namespace RealRT
{

    class Sphere : public Shape
    {
		public:
		    Sphere(const Material &mat, const Vector3D &center = {0.f, 0.f, 0.f}, float radius = 2.5f);
		    ~Sphere(void) = default;

		    float Intersect(const Ray &incident, bool &flipNormals) const;
		    Vector3D Normal(const Vector3D &pt) const;

		    float Radius(void) const;
		    Vector3D Center(void) const;

		private:
		    Vector3D _Center;
		    float _Radius;
    };

}
