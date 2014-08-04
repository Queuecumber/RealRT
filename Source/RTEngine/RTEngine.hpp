#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <stack>
#include <memory>
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

        void Resize(int width, int height);

        template <class Strategy>
        void Render(void)
        {
            Strategy strat(_World);

            Vector3D eyeLoc = {0.0, 0.0, -EyeDepth};

            for(int j = 0; j < _ScreenHeight; j++)
            {
                for(int i = 0; i < _ScreenWidth; i++)
                {
                    double x, y;
                    _ScreenToLogical(i, j, x, y);

                    Vector3D screenPosition = {x, y, 0.0};

                    Ray tracer(eyeLoc, screenPosition - eyeLoc);

                    Vector3D pixelColor = strat.Trace(tracer);

                    Vector3D suppressedColor = pixelColor.Clip(1.0);

                    int red = suppressedColor.I() * 255;
                    int green = suppressedColor.J() * 255;
                    int blue = suppressedColor.K() * 255;

                    _Screen[(j * _ScreenWidth + i) * 3] = red;
                    _Screen[(j * _ScreenWidth + i) * 3 + 1] = green;
                    _Screen[(j * _ScreenWidth + i) * 3 + 2] = blue;
                }
            }
        }

        template <class T>
        void AddWorldObject(T obj)
        {
            std::shared_ptr<Shape> shapePtr(new T(obj));

            _World.push_back(shapePtr);
        }

        template <class T>
        void RemoveWorldObject(T obj)
        {
            _World.remove_if([&obj](std::shared_ptr<Shape> el)
            {
                std::shared_ptr<T> testEl = std::dynamic_pointer_cast<T>(el);

                if(testEl != nullptr)
                    return *(testEl) == obj;
                else
                    return false;
            });
        }

        int ScreenWidth(void) const;
        int ScreenHeight(void) const;

        unsigned char *Screen(void) const;

    private:

        void _ScreenToLogical(const int i, const int j, double &x, double &y) const;

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
