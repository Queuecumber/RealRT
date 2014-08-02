#pragma once

#include "Shape.hpp"
#include "Vector3D.hpp"

namespace RealRT
{

    class Plane : public Shape
    {
        public:
            Plane(std::shared_ptr<const Material> mat, const Vector3D &normal = {0.0, 0.0, 1.0}, double d = 5.0);
			~Plane(void) = default;

		    double Intersect(const Ray &incident, bool &flipNormals) const;
		    Vector3D Normal(const Vector3D &pt) const;

		    bool IsLight(void) const;

		private:
            Vector3D _Normal;
		    double _D;
    };
}
