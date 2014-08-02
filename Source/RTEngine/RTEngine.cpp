#include "RTEngine.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include <iostream>
#include <algorithm>

namespace RealRT
{
    /*
      Window

      Utility structure that defines the
      start x,y for a window on the
      screen. This windows size is
      usually determined by RTEngine::DefaultWindowSize,
      however when the screensize is not a multiple of 10,
      the size can be smaller to account for the edges. Sizes
      for both axes should always be checked.

    */
    class Window
    {
        int left;
        int top;

        int right;
        int bottom;
    };

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

std::shared_ptr<RTEngine> RTEngine::_Instance(nullptr);

std::shared_ptr<RTEngine> RTEngine::Instantiate(int width, int height)
{
	//before instantiation, make sure there isnt already one
	//
	if(_Instance == nullptr)
	{
		_Instance.reset(new RTEngine(width, height));
	}
	else
	{
		if(width != _Instance->_ScreenWidth)
		{
			_Instance->_Resize(width,height);
		}
	}

	return _Instance;
}

RTEngine::RTEngine(int width,int height)
	: _MaxAsyncOperations(4)
{
	_Resize(width, height);
}

void RTEngine::_ScreenToLogical(const int i, const int j, float &x, float &y) const
{
    float widthScalar = float(LogicalWidth) / float(_ScreenWidth);
    float heightScalar = float(LogicalHeight) / float(_ScreenHeight);

    x = (i * widthScalar) - (LogicalWidth / 2.f);
    y = -((j * heightScalar) - (LogicalHeight / 2.f));
}

unsigned char *RTEngine::Screen(void) const
{
    return _Screen.get();
}

void RTEngine::CalculateScene()
{
	Vector3D eyeLoc = {0.0, 0.0, -EyeDepth};

	for(int j = 0; j < _ScreenHeight; j++)
	{
		for(int i = 0; i < _ScreenWidth; i++)
		{
			float x, y;
            _ScreenToLogical(i, j, x, y);

            Vector3D screenPosition = {x, y, 0.f};

            Ray tracer(eyeLoc, screenPosition - eyeLoc);

			Vector3D pixelColor = _RecursiveTrace(tracer, 0, 1.f);
			//vector<3,double> v3dColor = iterativeTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize()));

			Vector3D suppressedColor = pixelColor.Clip(1.f);

			int red = suppressedColor.I() * 255;
            int green = suppressedColor.J() * 255;
            int blue = suppressedColor.K() * 255;

            _Screen[(j * _ScreenWidth + i) * 3] = red;
            _Screen[(j * _ScreenWidth + i) * 3 + 1] = green;
            _Screen[(j * _ScreenWidth + i) * 3 + 2] = blue;
		}
	}
}

Vector3D RTEngine::_RecursiveTrace(const Ray &tracer, const int depth, const double refrIndex) const
{
    //begin traversing the world
    //
    double dist,closestDist = 1000000;
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
    Vector3D compositeColor = {0.f, 0.f, 0.f};
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
                Ray occlRay(intersectPoint + (0.0001 * toLight), toLight);
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

        if(refl > 0.0 && depth < MaxTraceIterations)
        {
            Vector3D reflectionDir;
            reflectionDir = tracer.Direction() - 2.0 * (tracer.Direction() * unitNormal) * unitNormal;

            Ray reflected(intersectPoint + (0.0001 * reflectionDir), reflectionDir);

            Vector3D reflCol = _RecursiveTrace(reflected, depth + 1, refrIndex);

            compositeColor += reflCol.Weight(refl * shapeMat->Color());
        }

        //apply refractions
        //
        if(refr > 0.0 && depth < MaxTraceIterations)
        {
            // This is basically Snell's law
            double relIndex = refrIndex / rindex;

            unitNormal = unitNormal * (closestFlipNormals ? -1.0 : 1.0);

            double cosI = - (unitNormal * tracer.Direction());
            double cosT2 = 1.0 - (relIndex * relIndex) * (1.0 - (cosI * cosI));
            if(cosT2 > 0.0)
            {
                Vector3D refractedDir = (relIndex * tracer.Direction()) + (relIndex * cosI - sqrt(cosT2)) * unitNormal;
                Ray refracted(intersectPoint + (0.0001 * refractedDir), refractedDir);

                Vector3D refrCol = _RecursiveTrace(refracted, depth + 1, rindex);

                compositeColor += refrCol.Weight(refr * shapeMat->Color());
            }
        }
    }

    return compositeColor;
}

// void RTEngine::CalculateSceneAsync()
// {
// 	//calculate ideal window sizes
// 	//
// 	DefaultWindowSizeI = SCREENWIDTH  / (int)(sqrt( float(MAXASYNCOPS) ));
// 	DefaultWindowSizeJ = SCREENHEIGHT / (int)(sqrt( float(MAXASYNCOPS) ));
//
// 	//Partition window and make window stack
// 	//
// 	for(int i = 0; i < SCREENWIDTH; i += DefaultWindowSizeI)
// 	{
// 		for(int j = 0; j < SCREENHEIGHT; j += DefaultWindowSizeJ)
// 		{
// 			Window *w = new Window;
//
// 			w->i = i;
// 			w->j = j;
//
// 			if(DefaultWindowSizeI > (SCREENWIDTH - i))
// 				w->sizei = (SCREENWIDTH - i);
// 			else
// 				w->sizei = DefaultWindowSizeI;
//
// 			if(DefaultWindowSizeJ > (SCREENHEIGHT - j))
// 				w->sizej = (SCREENHEIGHT - j);
// 			else
// 				w->sizej = DefaultWindowSizeJ;
//
//
// 			WindowStack.push(w);
// 		}
// 	}
//
// 	//create controlers
// 	//
// 	StackMutex  = CreateMutex(NULL,FALSE,"StackMutex");
// 	ScreenMutex = CreateMutex(NULL,FALSE,"ScreenMutex");
//
// 	HANDLE *threads = new HANDLE[MAXASYNCOPS];
//
// 	//begin rendering loop
// 	//
// 	INITDEBUG(".\\RTEngine.log");
// 	DEBLOG("Beginning Asyncronous Rendering");
// 	DEBLOG("Number of windows: "<<WindowStack.size());
// 	DEBLOG("Number of threads: "<<MAXASYNCOPS);
//
// 	for(int i = 0; i < MAXASYNCOPS; i++)
// 	{
// 		//spawn next thread
// 		//
// 		threads[i] = CreateThread(NULL,0,iterate,(LPVOID)i,NULL,NULL);
// 	}
//
// 	WaitForMultipleObjects(MAXASYNCOPS,threads,TRUE,INFINITE);
//
// 	DEBLOG("Rendering Complete");
// 	ENDDEBUG();
//
// 	for(int i = 0; i < MAXASYNCOPS; i++)
// 		CloseHandle(threads[i]);
//
// 	CloseHandle(StackMutex);
// 	CloseHandle(ScreenMutex);
// }

// DWORD WINAPI iterate(LPVOID lpParam)
// {
// 	RTEngine *engine = RTEngine::Instantiate();
//
// 	vector<3,double> v3dEyeLoc( 0.0,0.0,(double)-(RTEngine::EYEDEPTH) );
// 	bool terminate = false;
//
// 	while(!terminate)
// 	{
// 		WaitForSingleObject(engine->StackMutex,INFINITE);
// 		if(!engine->WindowStack.empty())
// 		{
// 			Window *win = engine->WindowStack.top();
// 			engine->WindowStack.pop();
// 			ReleaseMutex(engine->StackMutex);
//
// 			//dont log inside, io takes too long
// 			DEBLOG("\tThread "<<(int)lpParam<<" Rendering... Lines: "<<win->i<<" to "<<win->i + win->sizei<<"/"<<SCREENWIDTH<<" Columns: "<<win->j<<" to "<<win->j + win->sizej<<"/"<<SCREENHEIGHT);
//
// 			for(int i = win->i; i < (win->i + win->sizei); i++)
// 			{
// 				for(int j = win->j; j < (win->j + win->sizej); j++)
// 				{
// 					//calculate eye to screen vector
// 					//
// 					vector<2,double> screenPoint((double)i,(double)j);
// 					vector<2,double> logPoint(SCRNTOLOG(screenPoint));
//
// 					vector<3,double> v3dScreenPos( logPoint[0],logPoint[1],0.0 );
//
//
// 					//iterative trace
// 					//
// 					vector<3,double> v3dColor = engine->iterativeTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize()));
// 					//vector<3,double> v3dColor = engine->recursiveTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize()),0,1.0);
//
// 					gmath::clip(v3dColor,1.0);
//
// 					//composite color to screen (needs to be thread safe)
// 					//
// 					WaitForSingleObject(engine->ScreenMutex,INFINITE);
// 					engine->screen[i][j] = RGB( v3dColor[0] * 255,
// 												v3dColor[1] * 255,
// 												v3dColor[2] * 255 );
// 					ReleaseMutex(engine->ScreenMutex);
// 				}
// 			}
//
// 			delete win;
// 		}
// 		else
// 		{
// 			ReleaseMutex(engine->StackMutex);
// 			terminate = true;
// 		}
// 	}
//
// 	return 0;
//
// }

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

void RTEngine::AddWorldObject(std::shared_ptr<Shape> obj)
{
	_World.push_back(obj);
}

void RTEngine::RemoveWorldObject(std::shared_ptr<Shape> obj)
{
	_World.remove(obj);
}

void RTEngine::_Resize(int width, int height)
{
	_ScreenHeight = height;
	_ScreenWidth = width;

	_Screen.reset(new unsigned char[width * height * 3]);
    std::fill(_Screen.get(), _Screen.get() + (width * height * 3), 0);
}


RTEngine::~RTEngine(void)
{

}
