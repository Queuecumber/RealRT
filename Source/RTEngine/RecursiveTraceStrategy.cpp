#include "RecursiveTraceStrategy.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include <limits>
#include <cmath>

using namespace RealRT;

RecursiveTraceStrategy::RecursiveTraceStrategy(std::list<std::shared_ptr<Shape>> &world, int maxTraceIterations)
    : _World(world), _MaxTraceIterations(maxTraceIterations)
{

}

Vector3D RecursiveTraceStrategy::operator ()(const Ray &tracer, const int depth, const double refrIndex) const
{
    //begin traversing the world
    //
    double usableEpsilion = std::numeric_limits<float>::epsilon(); // Epsilon for a float will always produce a usable delta for doubles
    double dist,closestDist = std::numeric_limits<double>::infinity();
    std::shared_ptr<Shape> closest;
    bool closestFlipNormals;
    bool flipnormals = false;
    for(std::shared_ptr<Shape> s : _World)
    {
        //find the distance from the origin to the point of intersection
        //
        if((dist = s->Intersect(tracer, flipnormals)))
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

    //if the current world object is a light, do not apply shading
    //
    Vector3D compositeColor = {0.0, 0.0, 0.0};
    if(!closest->IsLight())
    {
        //calculate the position vector of the point of intersection using the line along the current ray
        //
        Vector3D intersectPoint = tracer.Origin() + (tracer.Direction() * closestDist);

        //get the unit normal at the point of intersection to apply shading
        Vector3D unitNormal = closest->Normal(intersectPoint);

        // Get the material of the shape
        std::shared_ptr<const Material> shapeMat = closest->SurfaceMaterial();

        //traverse the world again looking for lights
        //
        for(std::shared_ptr<Shape> l : _World)
        {
            //find only lights
            //
            if(l->IsLight())
            {
                std::shared_ptr<Sphere> asSphere = std::dynamic_pointer_cast<Sphere>(l);

                //retrieve the position vector of the light source (treated as a uniform pointline source)
                Vector3D lightCenter = asSphere->Center();

                //create a vector from the intersect point to the light source
                Vector3D toLight = (lightCenter - intersectPoint).Normalize();

                //calculate occlusion (for a pointlike source this is boolean)
                // Note that I'm adding a small offset in the direction we want to go so that we don't
                // intersect with the current shape
                Ray occlRay(intersectPoint + (usableEpsilion * toLight), toLight);
                double occlusion = 1.0;
                bool trash;
                for(std::shared_ptr<Shape> o : _World)
                {
                    if(o->Intersect(occlRay, trash) && !o->IsLight())
                    {
                        occlusion = 0.0;
                        break;
                    }
                }

                if(occlusion > 0)
                {
                    //apply lighting function
                    //
                    double radianceDist = shapeMat->BidirectionReflectanceDistributionFunction(tracer.Direction(), toLight, unitNormal);

                    std::shared_ptr<const Material> lightMat = l->SurfaceMaterial();
                    compositeColor += shapeMat->Color().Weight(lightMat->Color() * radianceDist * occlusion);
                }
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
    }

    return compositeColor;
}
