#include <list>
#include <memory>

namespace RealRT
{
    class Ray;
    class Vector3D;
    class Shape;
    class Light;

    class RecursiveTraceStrategy
    {
    public:
        RecursiveTraceStrategy(std::list<std::shared_ptr<Shape>> &shapes, std::list<std::shared_ptr<Light>> &lights, int maxTraceIterations = 6);
        Vector3D operator ()(const Ray &tracer, const int depth = 0, const double refrIndex = 1.0) const;

    private:
        const std::list<std::shared_ptr<Shape>> &_Shapes;
        const std::list<std::shared_ptr<Light>> &_Lights;
        const int _MaxTraceIterations;
    };
}
