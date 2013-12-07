#include "Shape.hpp"


using namespace RealRT;

Shape::Shape(const Material &mat)
    : _Material(mat)
{

}

Shape::~Shape(void)
{

}

inline bool Shape::IsLight(void) const
{
    return false;
}

const Material &Shape::Material(void) const
{
    return _Material;
}
