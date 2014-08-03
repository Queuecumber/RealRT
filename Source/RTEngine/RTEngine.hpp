#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <stack>
#include "Vector3D.hpp"
#include "Ray.hpp"

namespace RealRT
{
    class Shape;

    class RTEngine
    {
    public:
        static const int EyeDepth = 5;	// Distance from the origin to put the eye point on the Z-axis
        static const int MaxTraceIterations = 6; // Maximum number of reflection/refractions
        static const int LogicalSpaceSize = 20; // Target size for the logical space dimensions

        RTEngine(int width, int height);

        /*
            Render

            Fills in the virtual screen, ray tracing the scene
        */
        template <class Strategy>
        void Render(void);

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
        void _Resize(int width, int height);

        inline void _ScreenToLogical(const int i, const int j, double &x, double &y) const;

        std::list<std::shared_ptr<Shape>> _World;

        int _MaxAsyncOperations;
        std::thread _TracerThreads[];

        std::mutex _WindowMutex;
        //std::stack<Window> _WindowStack;

        std::mutex _ScreenMutex;

        double _LogicalWidth = 20;
        double _LogicalHeight = 20;

        int _ScreenWidth;
        int _ScreenHeight;

        std::unique_ptr<unsigned char[]> _Screen;
    };
}
