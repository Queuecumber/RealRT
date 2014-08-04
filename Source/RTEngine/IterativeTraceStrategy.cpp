#include "IterativeTraceStrategy.hpp"
#include "Vector3D.hpp"
#include "Ray.hpp"

namespace RealRT
{
    /*
        TRACENODE

        Utility structure, represents a
        node in the trace tree, this tree is kept implicitly
        on the stack

    */
    class TraceNode
    {
        Vector3D color;
        Vector3D shapeColor;

        Ray r;
        double refrIndex;
        double multiplier;
        bool eval;

        int	depth;

        TraceNode *parent = nullptr;
    };
}

using namespace RealRT;

IterativeTraceStrategy::IterativeTraceStrategy(std::list<std::shared_ptr<Shape>> &world, int maxTraceIterations)
    : _World(world), _MaxTraceIterations(maxTraceIterations)
{

}

Vector3D IterativeTraceStrategy::operator ()(const Ray &tracer, const int depth, const double refrIndex) const
{

}

// vector<3,double> RTEngine::iterativeTrace(ray &initial)
// {
// 	//Create primary node
// 	//
// 	TRACENODE primary;
// 	primary.r = initial;
// 	primary.refrIndex = 1.0;
// 	primary.multiplier = 1.0;
// 	primary.eval = false;
//
// 	primary.parent = NULL;
// 	primary.depth = 0;
//
// 	//Use a stack to hold the trace tree, similar to an LR shift-reduce parser.
// 	//Nodes on the stack are absorbed to the parent after they and their children are
// 	//processed. This gives a depth first traversal
// 	//
// 	stack<TRACENODE *> s;
// 	s.push(&primary);
// 	while(!s.empty())
// 	{
// 		//get the next node in the traversal
// 		//
// 		TRACENODE *current = s.top();
//
// 		//if the current node has been evaluated, then absorb it
// 		//to the parent and continue. This will also be the terminal case
// 		//and the result will be stored in the current rays color
// 		//
// 		if(current->eval)
// 		{
// 			//pop off stack
// 			s.pop();
//
// 			if(Absorb(current))
// 				return current->color;
// 			else
// 				continue;
// 		}
//
// 		//trace its ray
// 		//
// 		ray tracer = current->r;
//
// 		//begin traversing the world
// 		//
// 		shape3D *closest = NULL;
// 		double closestDist = 1000000;
// 		double dist;
// 		bool closestFlipNormals = false;
// 		bool flipnormals = false;
// 		for(std::vector<shape3D *>::iterator world_it = world.begin(); world_it != world.end(); world_it++)
// 		{
// 			//find the distance from the origin to the point of intersection
// 			//
// 			if( (dist = (*world_it)->HasIntersection(tracer,flipnormals)) )
// 			{
// 				if(dist < closestDist)
// 				{
// 					closestDist = dist;
// 					closest = (*world_it);
// 					closestFlipNormals = flipnormals;
// 				}
// 			}
// 		}
//
// 		PMATERIAL shapeMat = NULL;
// 		vector<3,double> rayColor;
// 		if(closest != NULL)
// 		{
// 			//if the current world object is a light, do not apply shading
// 			//
// 			if(!closest->IsLight())
// 			{
// 				//calculate the position vector of the point of intersection using the line along the current ray
// 				//
// 				vector<3,double> intersectPoint = tracer.GetStart() + (tracer.GetDirection() * closestDist);
//
// 				//get the unit normal at the point of intersection to apply shading
// 				vector<3,double> unitNormal = closest->GetNormal(intersectPoint);
//
// 				//traverse the world again looking for lights
// 				//
// 				for(std::vector<shape3D *>::iterator lights_it = world.begin(); lights_it != world.end(); lights_it++)
// 				{
// 					//find only lights
// 					//
// 					if((*lights_it)->IsLight())
// 					{
//
// 						double occlusion = 1.0;
//
// 						//retrieve the position vector of the light source (treated as a uniform pointline source)
// 						vector<3,double> lightCenter = ((sphere *)(*lights_it))->GetCenter();
//
// 						//create a vector from the intersect point to the light source
// 						vector<3,double> toLight = (lightCenter - intersectPoint).normalize();
//
//
// 						//calculate occlusion
// 						ray occlRay(intersectPoint + 0.0001 * toLight,toLight);
// 						bool trash;
// 						for(std::vector<shape3D *>::iterator occl_it = world.begin(); occl_it != world.end(); occl_it++)
// 						{
// 							if((*occl_it)->HasIntersection(occlRay,trash) && !(*occl_it)->IsLight())
// 							{
// 								occlusion = 0.0;
// 								break;
// 							}
// 						}
//
// 						//apply lighting function
// 						//
// 						shapeMat = closest->LockMaterial();
// 						double radianceDist = ( *(shapeMat->brdf) )(tracer.GetDirection(),toLight,unitNormal,(*shapeMat));
//
// 						PMATERIAL lightMat;
// 						lightMat = (*lights_it)->LockMaterial();
//
// 						rayColor += gmath::symmetricMultiply(shapeMat->color,lightMat->color * radianceDist * occlusion);
//
// 						(*lights_it)->UnlockMaterial(lightMat);
//
// 						closest->UnlockMaterial(shapeMat);
// 					}
// 				}
//
// 				//Set shaded color on node
// 				current->color = rayColor;
//
// 				//calculate child nodes (reflected node and refracted node)
// 				//and push them onto the stack
// 				//
// 				shapeMat = closest->LockMaterial();
// 				bool hasChildren = false;
//
// 				//apply reflections
// 				//
// 				if(shapeMat->reflectance > 0.0 && current->depth < TRACEDEPTH)
// 				{
// 					vector<3,double> reflectionDir = tracer.GetDirection() - 2.0 * (tracer.GetDirection() * unitNormal) * unitNormal;
//
// 					ray reflected(intersectPoint + 0.0001 * reflectionDir,reflectionDir);
//
// 					//fill in preprocess fields on the reflected node
// 					//
// 					TRACENODE *reflNode = new TRACENODE();
// 					reflNode->r = reflected;
// 					reflNode->shapeColor = shapeMat->color;
// 					reflNode->refrIndex = current->refrIndex;
// 					reflNode->multiplier = shapeMat->reflectance;
// 					reflNode->depth = current->depth + 1;
// 					reflNode->parent = current;
// 					reflNode->eval = false;
//
// 					s.push(reflNode);
//
// 					hasChildren = true;
//
// 				}
//
// 				//apply refractions
// 				//
// 				if(shapeMat->refractance > 0.0 && current->depth < TRACEDEPTH)
// 				{
// 					double relIndex = current->refrIndex / shapeMat->refIndex;
// 					unitNormal = unitNormal * (closestFlipNormals ? -1.0 : 1.0);	//remeber closestFlipNormals from the beginning?
// 					double cosI = - (unitNormal * tracer.GetDirection());
// 					double cosT2 = 1.0 - (relIndex * relIndex) * (1.0 - (cosI * cosI));
// 					if(cosT2 > 0.0)
// 					{
// 						vector<3,double> refractedDir = (relIndex * tracer.GetDirection()) + (relIndex * cosI - sqrt(cosT2)) * unitNormal;
// 						ray refracted(intersectPoint + 0.0001 * refractedDir,refractedDir);
//
// 						//fill in preprocess fields on the refracted node
// 						//
// 						TRACENODE *refrNode = new TRACENODE();
// 						refrNode->shapeColor = shapeMat->color;
// 						refrNode->r = refracted;
// 						refrNode->refrIndex = shapeMat->refIndex;
// 						refrNode->multiplier = shapeMat->refractance;
// 						refrNode->depth = current->depth + 1;
// 						refrNode->parent = current;
// 						refrNode->eval = false;
//
// 						s.push(refrNode);
//
// 						hasChildren = true;
// 					}
// 				}
//
// 				//unlock the material
// 				closest->UnlockMaterial(shapeMat);
//
// 				//evaluated this node
// 				current->eval = true;
//
// 				//if no children were generated, absorb
// 				if(!hasChildren)
// 				{
// 					//pop off stack
// 					s.pop();
//
// 					if(Absorb(current))
// 						return current->color;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			//ray failed to produce an intersection. Pop it off the stack and ignore its contribution
// 			s.pop();
// 		}
// 	}
//
// 	//there were no intersections, return black
// 	return vector<3,double>(0.0,0.0,0.0);
// }
//
// inline bool RTEngine::Absorb(TRACENODE *current)
// {
// 	//scale color to match parent
// 	vector<3,double> color = current->multiplier * gmath::symmetricMultiply(current->color , current->shapeColor);
//
// 	//get the parent node
// 	//
// 	TRACENODE *parent = current->parent;
// 	if(parent != NULL)
// 	{
// 		//if the parent exists, add the nodes color componant to it
// 		parent->color += color;
// 	}
// 	else
// 	{
// 		//found terminal case (root of trace tree)
// 		return true;
// 	}
//
// 	return false;
// }
