#include "Vector3D.hpp"
#include <cmath>
#include <iostream>
#include <exception>

using namespace RealRT;

Vector3D::Vector3D(double i, double j, double k)
{
    _I = i;
    _J = j;
    _K = k;
}

Vector3D::Vector3D(const std::array<double, 3> values)
{
    _I = values[0];
    _J = values[1];
    _K = values[2];
}

Vector3D::Vector3D(std::initializer_list<double> list)
{
    _I = list.begin()[0];
    _J = list.begin()[1];
    _K = list.begin()[2];
}

double Vector3D::I(void) const
{
    return _I;
}

double Vector3D::J(void) const
{
    return _J;
}

double Vector3D::K(void) const
{
    return _K;
}

Vector3D Vector3D::Add(const Vector3D &v) const
{
    return Vector3D(_I + v._I, _J + v._J, _K + v._K);
}

Vector3D Vector3D::Subtract(const Vector3D &v) const
{
    return Vector3D(_I - v._I, _J - v._J, _K - v._K);
}

Vector3D Vector3D::Normalize(void) const
{
    return Scale(1.0 / Magnitude());
}

double Vector3D::Distance(const Vector3D &rhs) const
{
    return std::sqrt(this->Dot(*this - rhs));
}

double Vector3D::Magnitude(void) const
{
    return std::sqrt(Dot(*this));
}

Vector3D Vector3D::Scale(double scalar) const
{
    return Vector3D(_I * scalar, _J * scalar, _K * scalar);
}

Vector3D Vector3D::Clip(double maxVal) const
{
    return Vector3D(_I > maxVal ? maxVal : _I,
                    _J > maxVal ? maxVal : _J,
                    _K > maxVal ? maxVal : _K);
}

Vector3D Vector3D::Weight(const Vector3D &rhs) const
{
    return Vector3D(_I * rhs._I, _J * rhs._J, _K * rhs._K);
}

double Vector3D::Dot(const Vector3D &rhs) const
{
    return (_I * rhs._I) + (_J * rhs._J) + (_K * rhs._K);
}

Vector3D Vector3D::Cross(const Vector3D &rhs) const
{
    return Vector3D(
       (_J * rhs._K) - (rhs._J * _K)  ,
     -((_I * rhs._K) - (rhs._I * _K)) ,
       (_I * rhs._J) - (rhs._I * _J) );
}

double Vector3D::operator [](int i) const
{
    switch(i)
    {
        case 0:
            return _I;

        case 1:
            return _J;

        case 2:
            return _K;

        default:
            throw std::out_of_range("Vector3D index out of bounds");
    }
}

bool Vector3D::operator ==(const Vector3D &rhs) const
{
    return _I == rhs._I && _J == rhs._J && _K == rhs._K;
}

bool Vector3D::operator !=(const Vector3D &rhs) const
{
    return !(*this == rhs);
}

bool Vector3D::operator <(const Vector3D &rhs) const
{
    return Magnitude() < rhs.Magnitude();
}

bool Vector3D::operator <=(const Vector3D &rhs) const
{
    return Magnitude() <= rhs.Magnitude();
}

bool Vector3D::operator >(const Vector3D &rhs) const
{
    return Magnitude() > rhs.Magnitude();
}

bool Vector3D::operator >=(const Vector3D &rhs) const
{
    return Magnitude() >= rhs.Magnitude();
}

Vector3D Vector3D::operator +(const Vector3D &rhs) const
{
    return Add(rhs);
}

Vector3D Vector3D::operator -(const Vector3D &rhs) const
{
    return Subtract(rhs);
}

Vector3D &Vector3D::operator +=(const Vector3D &rhs)
{
    _I += rhs._I;
    _J += rhs._J;
    _K += rhs._K;

    return *this;
}

Vector3D &Vector3D::operator -=(const Vector3D &rhs)
{
    _I -= rhs._I;
    _J -= rhs._J;
    _K -= rhs._K;

    return *this;
}

double Vector3D::operator *(const Vector3D &rhs) const
{
    return this->Dot(rhs);
}

Vector3D &Vector3D::operator *=(double scalar)
{
    _I *= scalar;
    _J *= scalar;
    _K *= scalar;

    return *this;
}

Vector3D Vector3D::operator %(const Vector3D &rhs) const
{
    return this->Cross(rhs);
}

Vector3D &Vector3D::operator %=(const Vector3D &rhs)
{
	_I =   (_J * rhs._K) - (rhs._J * _K) ;
    _J = -((_I * rhs._K) - (rhs._I * _K));
    _K =   (_I * rhs._J) - (rhs._I * _J) ;

    return *this;
}

namespace RealRT
{
    Vector3D operator *(double scalar, const Vector3D &v)
    {
        return v.Scale(scalar);
    }

    Vector3D operator *(const Vector3D &v, double scalar)
    {
        return v.Scale(scalar);
    }

    std::ostream &operator <<(std::ostream &os, const Vector3D &obj)
    {
        os << "[" << obj._I << " " << obj._J << " " << obj._K << "]";
        return os;
    }

    std::istream &operator >>(std::istream &is, Vector3D &obj)
    {
        double i, j, k;

        is.ignore(1, '[');
        is >> i >> j >> k;
        is.ignore(1, ']');

        obj._I = i;
        obj._J = j;
        obj._K = k;

        return is;
    }
}
