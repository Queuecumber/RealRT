#include "Shape.hpp"
#include "Material.hpp"
#include "Ray.hpp"

using namespace RealRT;

Shape::Shape(std::shared_ptr<const Material> mat)
    : _Material(mat)
{

}

inline bool Shape::IsLight(void) const
{
    return false;
}

const std::shared_ptr<const Material> Shape::SurfaceMaterial(void) const
{
    return _Material;
}
