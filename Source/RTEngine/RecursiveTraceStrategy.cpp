#include "RecursiveTraceStrategy.hpp"
#include "Material.hpp"
#include "Shape.hpp"
#include "Light.hpp"
#include "Ray.hpp"
#include <limits>
#include <cmath>

using namespace RealRT;

RecursiveTraceStrategy::RecursiveTraceStrategy(std::list<std::shared_ptr<Shape>> &shapes, std::list<std::shared_ptr<Light>> &lights, int maxTraceIterations)
    : _Shapes(shapes), _Lights(lights), _MaxTraceIterations(maxTraceIterations)
{

}

Vector3D RecursiveTraceStrategy::operator ()(const Ray &tracer, const int depth, const double refrIndex) const
{
    double usableEpsilion = std::numeric_limits<float>::epsilon(); // Epsilon for a float will always produce a usable delta for doubles (this is a little bit of a bad idea)

    //begin traversing the world
    //
    double closestDist = std::numeric_limits<double>::infinity();
    bool closestFlipNormals;
    std::shared_ptr<Shape> closest;

    for(std::shared_ptr<Shape> s : _Shapes)
    {
        bool flipnormals;
        double dist;

        //find the distance from the origin to the point of intersection
        //
        std::tie(dist, flipnormals) = s->Intersect(tracer);
        if(dist > 0)
        {
            if(dist > closestDist)
                continue;

            closestDist = dist;
            closest = s;
            closestFlipNormals = flipnormals;
        }
    }

    if(closest == nullptr)
        return {0.0, 0.0, 0.0};

    Vector3D compositeColor = {0.0, 0.0, 0.0};

    //calculate the position vector of the point of intersection using the line along the current ray
    //
    Vector3D intersectPoint = tracer.Origin() + (tracer.Direction() * closestDist);

    //get the unit normal at the point of intersection to apply shading
    Vector3D unitNormal = closest->Normal(intersectPoint);

    // Get the material of the shape
    std::shared_ptr<const Material> shapeMat = closest->SurfaceMaterial();

    //traverse the world again looking for lights
    //
    for(std::shared_ptr<Light> l : _Lights)
    {
        //retrieve the position vector of the light source (treated as a uniform pointline source)
        Vector3D lightCenter = l->Position();

        //create a vector from the intersect point to the light source
        Vector3D toLight = (lightCenter - intersectPoint).Normalize();

        //calculate occlusion (for a pointlike source this is boolean)
        // Note that I'm adding a small offset in the direction we want to go so that we don't
        // intersect with the current shape
        Ray occlRay(intersectPoint + (usableEpsilion * toLight), toLight);
        double visibility = 1.0; // occlusion = 1 - visibility
        for(std::shared_ptr<Shape> o : _Shapes)
        {
            double d;
            std::tie(d, std::ignore) = o->Intersect(occlRay);

            if(d > 0)
            {
                visibility = 0.0;
                break;
            }
        }

        if(visibility > 0)
        {
            //apply lighting function
            //
            double radianceDist = shapeMat->BidirectionReflectanceDistributionFunction(tracer.Direction(), toLight, unitNormal);

            std::shared_ptr<const Material> lightMat = l->LightMaterial();
            compositeColor += shapeMat->Color().Weight(lightMat->Color() * radianceDist * visibility);
        }
    }

    //apply reflections
    //
    double refl = shapeMat->Reflectance();
    double refr = shapeMat->Refractance();
    double rindex = shapeMat->IndexOfRefraction();

    if(refl > 0.0 && depth < _MaxTraceIterations)
    {
        Vector3D reflectionDir;
        reflectionDir = tracer.Direction() - 2.0 * (tracer.Direction() * unitNormal) * unitNormal;

        Ray reflected(intersectPoint + (usableEpsilion * reflectionDir), reflectionDir);

        Vector3D reflCol = (*this)(reflected, depth + 1, refrIndex);

        compositeColor += shapeMat->Color().Weight(refl * reflCol);
    }

    //apply refractions
    //
    if(refr > 0.0 && depth < _MaxTraceIterations)
    {
        // This is basically Snell's law
        double relIndex = refrIndex / rindex;

        unitNormal = unitNormal * (closestFlipNormals ? -1.0 : 1.0);

        double cosI = - (unitNormal * tracer.Direction());
        double cosT2 = 1.0 - (relIndex * relIndex) * (1.0 - (cosI * cosI));
        if(cosT2 > 0.0)
        {
            Vector3D refractedDir = (relIndex * tracer.Direction()) + (relIndex * cosI - sqrt(cosT2)) * unitNormal;
            Ray refracted(intersectPoint + (usableEpsilion * refractedDir), refractedDir);

            Vector3D refrCol = (*this)(refracted, depth + 1, rindex);

            compositeColor += shapeMat->Color().Weight(refr * refrCol);
        }
    }


    return compositeColor;
}
