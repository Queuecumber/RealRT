#pragma once

#include "Vector3D.hpp"

namespace RealRT
{

    class Material
    {
        public:

            Material(const Vector3D &color, double reflectance, double refractance, double indexOfRefraction, double diffuseScalar, double specularScalar);
            Material(const Material &cp);

            virtual double Brdf(const Vector3D &in, const Vector3D &out, const Vector3D &norm) const = 0;

            Vector3D Color(void) const;

            double Reflectance(void) const;

            double Refractance(void) const;

            double IndexOfRefraction(void) const;

            double DiffuseScalar(void) const;

            double SpecularScalar(void) const;

        private:

            Vector3D _Color;

            double _Reflectance;
            double _Refractance;
            double _IndexOfRefraction;

            double _DiffuseScalar;
            double _SpecularScalar;
    };

}
