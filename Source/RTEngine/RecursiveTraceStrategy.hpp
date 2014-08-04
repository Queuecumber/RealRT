#include <list>
#include <memory>

namespace RealRT
{
    class Ray;
    class Vector3D;
    class Shape;

    class RecursiveTraceStrategy
    {
    public:
        RecursiveTraceStrategy(std::list<std::shared_ptr<Shape>> &world, int maxTraceIterations = 6);
        Vector3D operator ()(const Ray &tracer, const int depth = 0, const double refrIndex = 1.0) const;

    private:
        const std::list<std::shared_ptr<Shape>> &_World;
        const int _MaxTraceIterations;
    };
}
