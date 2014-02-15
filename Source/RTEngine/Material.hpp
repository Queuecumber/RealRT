#pragma once

#include "Vector3D.hpp"

namespace RealRT
{

    class Material
    {
        public:

            Material(const Vector3D &color, float reflectance, float refractance, float indexOfRefraction, float diffuseScalar, float specularScalar);
            Material(const Material &cp);


            virtual float BidirectionReflectanceDistributionFunction(const Vector3D &in, const Vector3D &out, const Vector3D &norm) const = 0;

            Vector3D Color(void) const;

            float Reflectance(void) const;

            float Refractance(void) const;

            float IndexOfRefraction(void) const;

            float DiffuseScalar(void) const;

            float SpecularScalar(void) const;

        private:

            Vector3D _Color;

            float _Reflectance;
            float _Refractance;
            float _IndexOfRefraction;

            float _DiffuseScalar;
            float _SpecularScalar;
    };

}
