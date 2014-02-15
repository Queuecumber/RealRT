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

#include <thread>
#include <mutex>
#include <vector>
#include <stack>
#include "Vector3D.hpp"

namespace RealRT
{
    class Ray;
    class Shape;
    class Window;
    class TraceNode;

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
        static const int EyeDepth = 5;	// Distance from the origin to put the eye point on the Z-axis
        static const int MaxTraceIterations = 6; // Maximum number of reflection/refractions

        ~RTEngine(void);

        /*
            RTEngine *Instantiate(int width, int height)

            creates or returns the already existing RTEngine object for this application
            If the Engine object is created, its virtual screen has dimensions [width][height]
        */
        static RTEngine &Instantiate(int width = 1024, int height = 768);

        /*
            CalculateScene

            Fills in the virtual screen, ray tracing the scene
        */
        void CalculateScene();

        void CalculateSceneAsync();

        /*
            AddWorldObject(shape3D *obj)

            adds the shape pointed to by obj to the world
        */
        void AddWorldObject(const Shape &obj);

        /*
            AddWorldObject(shape3D *obj)

            removes the shape pointed to by obj from the world
        */
        void RemoveWorldObject(const Shape &obj);

        int ScreenWidth(void) const;
        int ScreenHeight(void) const;

        unsigned char *Screen(void) const;

    private:
        RTEngine(int width, int height);

        Vector3D _RecursiveTrace(Ray &tracer, int depth, double refrIndex);

        Vector3D _IterativeTrace(Ray &tracer);

        inline bool _Absorb(TraceNode &current);

        std::vector<Shape> _World;

        int _MaxAsyncOperations;
        std::thread _TracerThreads[];

        std::mutex _WindowMutex;
        std::stack<Window> _WindowStack;

        std::mutex _ScreenMutex;
        int _ScreenWidth, _ScreenHeight;
        unsigned char _Screen[][3];
    };

}
