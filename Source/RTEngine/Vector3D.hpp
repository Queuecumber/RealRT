#pragma once

#include <iosfwd>
#include <initializer_list>
#include <array>

namespace RealRT
{
    class Vector3D
    {
        public:
            Vector3D(void) = default;

            Vector3D(double i, double j, double k);

            explicit Vector3D(const std::array<double, 3> values);

            Vector3D(std::initializer_list<double> list);

            double I(void) const;

            double J(void) const;

            double K(void) const;

            Vector3D Add(const Vector3D &v) const;

            Vector3D Subtract(const Vector3D &v) const;

            Vector3D Normalize(void) const;

            double Distance(const Vector3D &rhs) const;

            double Magnitude(void) const;

            Vector3D Scale(double scalar) const;

            Vector3D Clip(double maxVal) const;

            Vector3D Weight(const Vector3D &dimWeights) const;

            double Dot(const Vector3D &rhs) const;

            Vector3D Cross(const Vector3D &rhs) const;

            double operator [](int i) const;

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

            friend Vector3D operator *(double scalar, const Vector3D &v);

            friend Vector3D operator *(const Vector3D &v, double scalar);

            double operator *(const Vector3D &rhs) const;

            Vector3D &operator *=(double scalar);

            Vector3D operator %(const Vector3D &rhs) const;

            Vector3D &operator %=(const Vector3D &rhs);

            friend std::ostream &operator<<(std::ostream &os, const Vector3D &obj);

            friend std::istream &operator>>(std::istream &is, Vector3D &obj);

        private:

            double _I = 0.0, _J = 0.0, _K = 0.0;
	};
}
