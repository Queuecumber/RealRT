#pragma once

#include "Shape.hpp"

namespace RealRT
{

    class Plane : public Shape
    {
		public:
		    Plane(const Material &mat, const cv::Vector3D &normal = {0.f, 0.f, 0.f}, float d = 5.f);
			~Plane(void) = default;

		    float Intersect(const Ray &incident, bool &flipNormals) const;
		    Vector3D Normal(const Vector3D &pt) const;        

		    bool IsLight(void) const;

		private:
		    Vector3D _Normal;
		    float _D;
    };

}
