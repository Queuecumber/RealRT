#include "RTEngine.h"
#include <typeinfo>
#include <string>
#include <iostream>
#include <stack>
#include <condebug.h>

using std::string;
using std::endl;
using std::cerr;
using std::stack;


FILE *activeLog = NULL;

RTEngine *instance = NULL;	//pointer to the only instance of RTEngine

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

struct Window
{
	int i;
	int j;
	int sizei;
	int sizej;
};

/*
	TRACENODE

	Utility structure, represents a 
	node in the trace tree, this tree is kept implicitly
	on the stack

*/

struct TRACENODE
{
	vector<3,double> color;
	vector<3,double> shapeColor;

	ray		r;
	double	refrIndex;
	double	multiplier;
	bool	eval;

	int	depth;

	TRACENODE *parent;
	//TRACENODE *left,*right;

	TRACENODE(){parent = NULL;}//left=NULL;right=NULL;}
};

RTEngine *RTEngine::Instantiate(int width, int height)
{
	//before instantiation, make sure there isnt already one
	//
	if(!instance)
	{
		instance = new RTEngine(width,height);
	}
	else
	{
		if(width != instance->SCREENWIDTH)
		{
			instance->resize(width,height);
		}
	}
	
	return instance;
}  

RTEngine *RTEngine::Instantiate()
{
	return instance;
}

RTEngine::RTEngine(int width,int height)
	:world(), SCREENWIDTH(width), SCREENHEIGHT(height) 
{
	//create the virtual screen
	//
	screen = new LPCOLORREF[width];

	for(int i = 0; i < width; i++)
		screen[i] = new COLORREF[height];

	//fill the screen in with black
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			screen[i][j] = 0x00000000;

	MAXASYNCOPS = pow((double)omp_get_num_procs(),2.0);
}

void RTEngine::CalculateScene()
{
	INITDEBUG(".\\RTEngine.log");
	DEBLOG("Beginning Rendering");

	vector<3,double> v3dEyeLoc(0.0,0.0,(double)-EYEDEPTH);

	for(int i = 0; i < SCREENWIDTH; i++) 
	{
		for(int j = 0; j < SCREENHEIGHT; j++)
		{
			vector<2,double> screenPoint((double)i,(double)j);
			vector<2,double> logPoint(SCRNTOLOG(screenPoint));

			vector<3,double> v3dScreenPos( logPoint[0],logPoint[1],0.0 );

			//vector<3,double> v3dColor = recursiveTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize() ),0,1.0);
			vector<3,double> v3dColor = iterativeTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize())); 

			gmath::clip(v3dColor,1.0);

			screen[i][j] = RGB( v3dColor[0] * 255,
							    v3dColor[1] * 255,
							    v3dColor[2] * 255 );
		}

		if(!(i % 100) )			
			DEBLOG("\tRendering... Line: "<<i<<"/"<<SCREENWIDTH);
	}
	DEBLOG("Rendering Complete");
	ENDDEBUG();
}

void RTEngine::CalculateSceneAsync()
{	
	//calculate ideal window sizes
	//
	DefaultWindowSizeI = SCREENWIDTH  / (int)(sqrt( float(MAXASYNCOPS) ));
	DefaultWindowSizeJ = SCREENHEIGHT / (int)(sqrt( float(MAXASYNCOPS) ));

	//Partition window and make window stack
	//
	for(int i = 0; i < SCREENWIDTH; i += DefaultWindowSizeI)
	{	  
		for(int j = 0; j < SCREENHEIGHT; j += DefaultWindowSizeJ)
		{
			Window *w = new Window;

			w->i = i;
			w->j = j;
			
			if(DefaultWindowSizeI > (SCREENWIDTH - i))
				w->sizei = (SCREENWIDTH - i);
			else
				w->sizei = DefaultWindowSizeI;

			if(DefaultWindowSizeJ > (SCREENHEIGHT - j))
				w->sizej = (SCREENHEIGHT - j);
			else
				w->sizej = DefaultWindowSizeJ;
			

			WindowStack.push(w);
		}		
	}

	//create controlers
	//
	StackMutex  = CreateMutex(NULL,FALSE,"StackMutex");
	ScreenMutex = CreateMutex(NULL,FALSE,"ScreenMutex");

	HANDLE *threads = new HANDLE[MAXASYNCOPS];

	//begin rendering loop
	//
	INITDEBUG(".\\RTEngine.log");
	DEBLOG("Beginning Asyncronous Rendering");
	DEBLOG("Number of windows: "<<WindowStack.size());
	DEBLOG("Number of threads: "<<MAXASYNCOPS);

	for(int i = 0; i < MAXASYNCOPS; i++)
	{
		//spawn next thread
		//
		threads[i] = CreateThread(NULL,0,iterate,(LPVOID)i,NULL,NULL);
	}

	WaitForMultipleObjects(MAXASYNCOPS,threads,TRUE,INFINITE);

	DEBLOG("Rendering Complete");
	ENDDEBUG();

	for(int i = 0; i < MAXASYNCOPS; i++)
		CloseHandle(threads[i]);

	CloseHandle(StackMutex);
	CloseHandle(ScreenMutex);
}

DWORD WINAPI iterate(LPVOID lpParam)
{
	RTEngine *engine = RTEngine::Instantiate();

	vector<3,double> v3dEyeLoc( 0.0,0.0,(double)-(RTEngine::EYEDEPTH) );
	bool terminate = false;

	while(!terminate)
	{
		WaitForSingleObject(engine->StackMutex,INFINITE);
		if(!engine->WindowStack.empty())
		{
			Window *win = engine->WindowStack.top();
			engine->WindowStack.pop();
			ReleaseMutex(engine->StackMutex);
		
			//dont log inside, io takes too long
			DEBLOG("\tThread "<<(int)lpParam<<" Rendering... Lines: "<<win->i<<" to "<<win->i + win->sizei<<"/"<<SCREENWIDTH<<" Columns: "<<win->j<<" to "<<win->j + win->sizej<<"/"<<SCREENHEIGHT);

			for(int i = win->i; i < (win->i + win->sizei); i++)
			{
				for(int j = win->j; j < (win->j + win->sizej); j++)
				{
					//calculate eye to screen vector
					//
					vector<2,double> screenPoint((double)i,(double)j);
					vector<2,double> logPoint(SCRNTOLOG(screenPoint));

					vector<3,double> v3dScreenPos( logPoint[0],logPoint[1],0.0 );


					//iterative trace
					//
					vector<3,double> v3dColor = engine->iterativeTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize())); 
					//vector<3,double> v3dColor = engine->recursiveTrace(ray(v3dEyeLoc,(v3dScreenPos - v3dEyeLoc).normalize()),0,1.0); 

					gmath::clip(v3dColor,1.0);

					//composite color to screen (needs to be thread safe)
					//
					WaitForSingleObject(engine->ScreenMutex,INFINITE);
					engine->screen[i][j] = RGB( v3dColor[0] * 255,
												v3dColor[1] * 255,
												v3dColor[2] * 255 );
					ReleaseMutex(engine->ScreenMutex);
				}
			}

			delete win;
		}
		else
		{
			ReleaseMutex(engine->StackMutex);
			terminate = true;
		}
	}

	return 0;

}

vector<3,double> RTEngine::iterativeTrace(ray &initial)
{
	//Create primary node
	//
	TRACENODE primary;
	primary.r = initial;
	primary.refrIndex = 1.0;
	primary.multiplier = 1.0;
	primary.eval = false;

	primary.parent = NULL;
	primary.depth = 0;

	//Use a stack to hold the trace tree, similar to an LR shift-reduce parser.
	//Nodes on the stack are absorbed to the parent after they and their children are
	//processed. This gives a depth first traversal
	//
	stack<TRACENODE *> s;
	s.push(&primary);
	while(!s.empty())
	{
		//get the next node in the traversal
		//
		TRACENODE *current = s.top();

		//if the current node has been evaluated, then absorb it 
		//to the parent and continue. This will also be the terminal case
		//and the result will be stored in the current rays color
		//
		if(current->eval)
		{
			//pop off stack
			s.pop();

			if(Absorb(current))
				return current->color;
			else
				continue;
		}

		//trace its ray
		//
		ray tracer = current->r;

		//begin traversing the world
		//
		shape3D *closest = NULL;
		double closestDist = 1000000;
		double dist;
		bool closestFlipNormals = false;
		bool flipnormals = false;
		for(std::vector<shape3D *>::iterator world_it = world.begin(); world_it != world.end(); world_it++)
		{
			//find the distance from the origin to the point of intersection
			//
			if( (dist = (*world_it)->HasIntersection(tracer,flipnormals)) )
			{
				if(dist < closestDist)
				{
					closestDist = dist;
					closest = (*world_it);
					closestFlipNormals = flipnormals;
				}
			}
		}

		PMATERIAL shapeMat = NULL;
		vector<3,double> rayColor;
		if(closest != NULL)
		{
			//if the current world object is a light, do not apply shading
			//
			if(!closest->IsLight())
			{
				//calculate the position vector of the point of intersection using the line along the current ray
				//
				vector<3,double> intersectPoint = tracer.GetStart() + (tracer.GetDirection() * closestDist);

				//get the unit normal at the point of intersection to apply shading
				vector<3,double> unitNormal = closest->GetNormal(intersectPoint);
				
				//traverse the world again looking for lights
				//
				for(std::vector<shape3D *>::iterator lights_it = world.begin(); lights_it != world.end(); lights_it++)
				{
					//find only lights
					//
					if((*lights_it)->IsLight())
					{

						double occlusion = 1.0;

						//retrieve the position vector of the light source (treated as a uniform pointline source)
						vector<3,double> lightCenter = ((sphere *)(*lights_it))->GetCenter();

						//create a vector from the intersect point to the light source
						vector<3,double> toLight = (lightCenter - intersectPoint).normalize();
						

						//calculate occlusion
						ray occlRay(intersectPoint + 0.0001 * toLight,toLight);
						bool trash;
						for(std::vector<shape3D *>::iterator occl_it = world.begin(); occl_it != world.end(); occl_it++)
						{
							if((*occl_it)->HasIntersection(occlRay,trash) && !(*occl_it)->IsLight())
							{
								occlusion = 0.0;
								break;
							}
						}

						//apply lighting function
						//
						shapeMat = closest->LockMaterial();
						double radianceDist = ( *(shapeMat->brdf) )(tracer.GetDirection(),toLight,unitNormal,(*shapeMat));

						PMATERIAL lightMat;
						lightMat = (*lights_it)->LockMaterial();

						rayColor += gmath::symmetricMultiply(shapeMat->color,lightMat->color * radianceDist * occlusion);

						(*lights_it)->UnlockMaterial(lightMat);

						closest->UnlockMaterial(shapeMat);
					}
				}

				//Set shaded color on node
				current->color = rayColor;

				//calculate child nodes (reflected node and refracted node)
				//and push them onto the stack
				//
				shapeMat = closest->LockMaterial();
				bool hasChildren = false;

				//apply reflections
				//
				if(shapeMat->reflectance > 0.0 && current->depth < TRACEDEPTH)
				{
					vector<3,double> reflectionDir = tracer.GetDirection() - 2.0 * (tracer.GetDirection() * unitNormal) * unitNormal;

					ray reflected(intersectPoint + 0.0001 * reflectionDir,reflectionDir); 

					//fill in preprocess fields on the reflected node
					//
					TRACENODE *reflNode = new TRACENODE();
					reflNode->r = reflected;
					reflNode->shapeColor = shapeMat->color;
					reflNode->refrIndex = current->refrIndex;
					reflNode->multiplier = shapeMat->reflectance;
					reflNode->depth = current->depth + 1;
					reflNode->parent = current;
					reflNode->eval = false;

					s.push(reflNode);

					hasChildren = true;
					
				}				

				//apply refractions
				//
				if(shapeMat->refractance > 0.0 && current->depth < TRACEDEPTH)
				{
					double relIndex = current->refrIndex / shapeMat->refIndex;
					unitNormal = unitNormal * (closestFlipNormals ? -1.0 : 1.0);	//remeber closestFlipNormals from the beginning?
					double cosI = - (unitNormal * tracer.GetDirection());
					double cosT2 = 1.0 - (relIndex * relIndex) * (1.0 - (cosI * cosI));
					if(cosT2 > 0.0)
					{
						vector<3,double> refractedDir = (relIndex * tracer.GetDirection()) + (relIndex * cosI - sqrt(cosT2)) * unitNormal;
						ray refracted(intersectPoint + 0.0001 * refractedDir,refractedDir);

						//fill in preprocess fields on the refracted node
						//
						TRACENODE *refrNode = new TRACENODE();
						refrNode->shapeColor = shapeMat->color;
						refrNode->r = refracted;
						refrNode->refrIndex = shapeMat->refIndex;
						refrNode->multiplier = shapeMat->refractance;
						refrNode->depth = current->depth + 1;
						refrNode->parent = current;
						refrNode->eval = false;

						s.push(refrNode);

						hasChildren = true;
					}
				}

				//unlock the material
				closest->UnlockMaterial(shapeMat);

				//evaluated this node
				current->eval = true;

				//if no children were generated, absorb
				if(!hasChildren)
				{
					//pop off stack
					s.pop();

					if(Absorb(current))
						return current->color;
				}
			}
		}
		else
		{
			//ray failed to produce an intersection. Pop it off the stack and ignore its contribution
			s.pop();
		}
	}

	//there were no intersections, return black
	return vector<3,double>(0.0,0.0,0.0);
}

inline bool RTEngine::Absorb(TRACENODE *current)
{
	//scale color to match parent
	vector<3,double> color = current->multiplier * gmath::symmetricMultiply(current->color , current->shapeColor);

	//get the parent node
	//
	TRACENODE *parent = current->parent;
	if(parent != NULL)
	{
		//if the parent exists, add the nodes color componant to it
		parent->color += color;
	}
	else
	{
		//found terminal case (root of trace tree)
		return true;
	}

	return false;
}

vector<3,double> RTEngine::recursiveTrace(ray &tracer, int depth, double refrIndex)
{
	//begin traversing the world
	//
	double dist,closestDist = 1000000;
	shape3D *closest = NULL;
	bool closestFlipNormals;
	bool flipnormals = false;
	for(std::vector<shape3D *>::iterator world_it = world.begin(); world_it != world.end(); world_it++)
	{
		//find the distance from the origin to the point of intersection
		//
		if( (dist = (*world_it)->HasIntersection(tracer,flipnormals)) )
		{
			if(dist > closestDist)
				continue;

			closestDist = dist;
			closest = (*world_it);
			closestFlipNormals = flipnormals;
		}
	}

	if(closest == NULL)
		return vector<3,double>(0.0,0.0,0.0); 

	//if the current world object is a light, do not apply shading
	//
	PMATERIAL shapeMat = NULL;
	vector<3,double> compositeColor;
	if(!closest->IsLight())
	{
		//calculate the position vector of the point of intersection using the line along the current ray
		//
		vector<3,double> intersectPoint = tracer.GetStart() + (tracer.GetDirection() * closestDist);

		//get the unit normal at the point of intersection to apply shading
		vector<3,double> unitNormal = closest->GetNormal(intersectPoint);
		
		//traverse the world again looking for lights
		//
		for(std::vector<shape3D *>::iterator lights_it = world.begin(); lights_it != world.end(); lights_it++)
		{
			//find only lights
			//
			if((*lights_it)->IsLight())
			{

				double occlusion = 1.0;

				//retrieve the position vector of the light source (treated as a uniform pointline source)
				vector<3,double> lightCenter = ((sphere *)(*lights_it))->GetCenter();

				//create a vector from the intersect point to the light source
				vector<3,double> toLight = (lightCenter - intersectPoint).normalize();

				//calculate occlusion (for a pointlike source this is boolean)
				ray occlRay(intersectPoint + 0.0001 * toLight,toLight);
				bool trash;
				for(std::vector<shape3D *>::iterator occl_it = world.begin(); occl_it != world.end(); occl_it++)
				{
					if((*occl_it)->HasIntersection(occlRay,trash) && !(*occl_it)->IsLight())
					{
						occlusion = 0.0;
						break;
					}
				}

				//apply lighting function (Bidirectional Reflectance Distribution Function see wikipedia.org)
				//
				shapeMat = closest->LockMaterial();
				double radianceDist = ( *(shapeMat->brdf) )(tracer.GetDirection(),toLight,unitNormal,(*shapeMat));

				PMATERIAL lightMat;
				lightMat = (*lights_it)->LockMaterial();

				compositeColor += gmath::symmetricMultiply(shapeMat->color,lightMat->color * radianceDist * occlusion);

				(*lights_it)->UnlockMaterial(lightMat);

				closest->UnlockMaterial(shapeMat);
			}
		}

		//apply reflections 
		//

		shapeMat = closest->LockMaterial();
		double refl = shapeMat->reflectance;
		double refr = shapeMat->refractance;
		double rindex = shapeMat->refIndex;
		closest->UnlockMaterial(shapeMat);

		if(refl > 0.0 && depth < TRACEDEPTH)
		{
			vector<3,double> reflectionDir;
			reflectionDir = tracer.GetDirection() - 2.0 * (tracer.GetDirection() * unitNormal) * unitNormal;

			ray reflected(intersectPoint + 0.0001 * reflectionDir,reflectionDir); 

			vector<3,double> reflCol = recursiveTrace(reflected,depth + 1,refrIndex);

			shapeMat = closest->LockMaterial();
			compositeColor += gmath::symmetricMultiply(reflCol, refl * shapeMat->color);
			closest->UnlockMaterial(shapeMat);
		}

		//apply refractions
		//
		if(refr > 0.0 && depth < TRACEDEPTH)
		{
			double relIndex = refrIndex / rindex;
			unitNormal = unitNormal * (closestFlipNormals ? -1.0 : 1.0);
			double cosI = - (unitNormal * tracer.GetDirection());
			double cosT2 = 1.0 - (relIndex * relIndex) * (1.0 - (cosI * cosI));
			if(cosT2 > 0.0)
			{
				vector<3,double> refractedDir;
				refractedDir = (relIndex * tracer.GetDirection()) + (relIndex * cosI - sqrt(cosT2)) * unitNormal;
				ray refracted(intersectPoint + 0.0001 * refractedDir,refractedDir);

				vector<3,double> refrCol = recursiveTrace(refracted,depth + 1,rindex);

				shapeMat = closest->LockMaterial();
				compositeColor += gmath::symmetricMultiply(refrCol, refr * shapeMat->color);
				closest->UnlockMaterial(shapeMat);
			}
		}
	}

    return compositeColor;
}

void RTEngine::Render(HDC hDC)
{
	//set every pixel to the same value as the virtual screen, note that
	//hDC should be a back buffer for smoothest rendering
	for(int i = 0; i < SCREENWIDTH; i++)
	{
		for(int j = 0; j < SCREENHEIGHT; j++)
		{
           SetPixel(hDC,i,j,screen[i][j]);
		}
	}
}

void RTEngine::AddWorldObject(shape3D *obj)
{
	world.push_back(obj);
}

void RTEngine::RemoveWorldObject(shape3D *obj)
{
	std::vector<shape3D *>::iterator i;
	for(i = world.begin(); i != world.end(); i++)
		if(*i == obj)
			break;

	world.erase(i);
}

void RTEngine::resize(int width, int height)
{
	SCREENHEIGHT = height;
	SCREENWIDTH = width;

	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			screen[width][height] = 0x00000000;
}


RTEngine::~RTEngine(void)
{
}

