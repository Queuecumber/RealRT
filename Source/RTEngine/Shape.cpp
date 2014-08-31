#include "Shape.hpp"
#include "Material.hpp"

using namespace RealRT;

Shape::Shape(std::shared_ptr<const Material> mat)
    : _Material(mat)
{

}

const std::shared_ptr<const Material> Shape::SurfaceMaterial(void) const
{
    return _Material;
}
