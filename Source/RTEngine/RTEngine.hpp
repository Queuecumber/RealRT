/*

	RTEngine

	Ray Tracing engine for furture real time
	use via nVidia CUDA technology

	link to RTEngine.lib, include RTEngine.h
*/

/*
	RTEngine.h

	Main include for the engine and definition of the
	RTEngine class

*/

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <omp.h>
#include <windows.h>
#include <vector>
#include <stack>

#include <gmath.h>
using gmath::vector;


//Engine includes
//
#include "ray.h"
#include "shape3D.h"
#include "sphere.h"
#include "plane3d.h"
#include "light.h"
#include "SphereicalLight.h"

//forward declarations
//
struct Window;
struct TRACENODE;

/*
	class RTEngine

	Facilitates all ray tracing.
	calculates the scene and renders it to 
	a given device context

	Only one instance can exist per application
	and is gotten by calling the class function Instantiate,
	note that the constructor is private preventing 
	creation of extra RTEngine objects
*/

class RTEngine
{
public:
	static const int EYEDEPTH = 5;	//distance from the origion to put the eye point
	static const int TRACEDEPTH = 6;
	int DefaultWindowSizeI;
	int DefaultWindowSizeJ;
	int SCREENWIDTH;
	int SCREENHEIGHT;

	~RTEngine(void);

	/*
		RTEngine *Instantiate(int width, int height)

		creates or returns the already existing RTEngine object for this application
		If the Engine object is created, its virtual screen has dimensions [width][height]
	*/
	static RTEngine * _cdecl Instantiate(int width, int height);
	static RTEngine * _cdecl Instantiate();

	/*
		CalculateScene

		Fills in the virtual screen, ray tracing the scene
	*/
	void CalculateScene();

	void CalculateSceneAsync();

	/*
		Render(HDC hDC)

		Renders the virtual screen to the parameter passed device context,
		this should be a memory device context to be used as a backbuffer but
		can be any device context
	*/
	void Render(HDC hDC);
	
	/*
		AddWorldObject(shape3D *obj)

		adds the shape pointed to by obj to the world
	*/
	void AddWorldObject(shape3D *obj);

	/*
		AddWorldObject(shape3D *obj)

		removes the shape pointed to by obj from the world
	*/
	void RemoveWorldObject(shape3D *obj);

	void resize(int width, int height);

	friend DWORD WINAPI iterate(LPVOID lpParam);

private:
	RTEngine(int width,int height);

	vector<3,double> recursiveTrace(ray &tracer, int depth, double refrIndex);
	
	vector<3,double> iterativeTrace(ray &tracer);

	inline bool Absorb(TRACENODE *current);

	std::vector<shape3D *> world;

	COLORREF **screen;

	int MAXASYNCOPS;
	HANDLE StackMutex;
	HANDLE ScreenMutex;

	std::stack<Window *> WindowStack;
};
