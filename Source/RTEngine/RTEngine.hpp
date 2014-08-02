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
#include <list>
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
        static const int LogicalSpaceSize = 20;

        ~RTEngine(void);

        /*
            RTEngine *Instantiate(int width, int height)

            creates or returns the already existing RTEngine object for this application
            If the Engine object is created, its virtual screen has dimensions [width][height]
        */
        static std::shared_ptr<RTEngine> Instantiate(int width = 1024, int height = 768);

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
        void AddWorldObject(std::shared_ptr<Shape> obj);

        /*
            AddWorldObject(shape3D *obj)

            removes the shape pointed to by obj from the world
        */
        void RemoveWorldObject(std::shared_ptr<Shape> obj);

        int ScreenWidth(void) const;
        int ScreenHeight(void) const;

        unsigned char *Screen(void) const;

    private:
        RTEngine(int width, int height);

        void _Resize(int width, int height);

        Vector3D _RecursiveTrace(const Ray &tracer, const int depth, const double refrIndex) const;

        Vector3D _IterativeTrace(Ray &tracer);

        inline bool _Absorb(TraceNode &current);

        inline void _ScreenToLogical(const int i, const int j, double &x, double &y) const;

        std::list<std::shared_ptr<Shape>> _World;

        int _MaxAsyncOperations;
        std::thread _TracerThreads[];

        std::mutex _WindowMutex;
        std::stack<Window> _WindowStack;

        std::mutex _ScreenMutex;
        int _ScreenWidth, _ScreenHeight;
        std::unique_ptr<unsigned char[]> _Screen;

        static std::shared_ptr<RTEngine> _Instance;

        double _LogicalWidth = 20;
        double _LogicalHeight = 20;
    };

}
