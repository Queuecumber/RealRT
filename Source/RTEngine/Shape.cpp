#include "Shape.hpp"
#include "Material.hpp"
#include "Ray.hpp"

using namespace RealRT;

Shape::Shape(const Material &mat)
    : _Material(mat)
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
