#pragma once

#include "Shape.hpp"
#include "Vector3D.hpp"

namespace RealRT
{

    class Plane : public Shape
    {
        public:
            Plane(const Material &mat, const Vector3D &normal = Vector3D({0.f, 0.f, 1.f}), float d = 5.f);
			~Plane(void) = default;

		    float Intersect(const Ray &incident, bool &flipNormals) const;
		    Vector3D Normal(const Vector3D &pt) const;        

		    bool IsLight(void) const;

		private:
            Vector3D _Normal;
		    float _D;
    };
}
