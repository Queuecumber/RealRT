#pragma once

#include <iosfwd>
#include <initializer_list>

namespace RealRT
{
    class Vector3D
    {
        public:
            Vector3D(void) = default;

            Vector3D(float i, float j, float k);

            explicit Vector3D(const float *values);

            Vector3D(std::initializer_list<float> list);

            float I(void) const;

            float J(void) const;

            float K(void) const;

            Vector3D Add(const Vector3D &v) const;

            Vector3D Subtract(const Vector3D &v) const;

            Vector3D Normalize(void) const;

            float Distance(const Vector3D &rhs) const;

            float Magnitude(void) const;

            Vector3D Scale(float scalar) const;

            Vector3D Clip(float maxVal) const;

            Vector3D Weight(const Vector3D &dimWeights) const;

            float Dot(const Vector3D &rhs) const;

            Vector3D Cross(const Vector3D &rhs) const;

            float operator [](int i) const;

            bool operator ==(const Vector3D &rhs) const;

            bool operator !=(const Vector3D &rhs) const;

            bool operator <(const Vector3D &rhs) const;

            bool operator <=(const Vector3D &rhs) const;

            bool operator >(const Vector3D &rhs) const;

            bool operator >=(const Vector3D &rhs) const;

            Vector3D operator +(const Vector3D &rhs) const;

            Vector3D operator -(const Vector3D &rhs) const;

            Vector3D &operator +=(const Vector3D &rhs);

            Vector3D &operator -=(const Vector3D &rhs);

            friend Vector3D operator *(float scalar, const Vector3D &v);

            friend Vector3D operator *(const Vector3D &v, float scalar);

            float operator *(const Vector3D &rhs) const;

            Vector3D &operator *=(float scalar);

            Vector3D operator %(const Vector3D &rhs) const;

            Vector3D &operator %=(const Vector3D &rhs);

            friend std::ostream &operator<<(std::ostream &os, const Vector3D &obj);

            friend std::istream &operator>>(std::istream &is, Vector3D &obj);

        private:

            float _I = 0.f, _J = 0.f, _K = 0.f;
        };

}
