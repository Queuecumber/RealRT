#include "RTEngine.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "RecursiveTraceStrategy.hpp"
#include "IterativeTraceStrategy.hpp"
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
}

using namespace RealRT;

RTEngine::RTEngine(int width,int height)
	: _MaxAsyncOperations(4)
{
	_Resize(width, height);
}

void RTEngine::_ScreenToLogical(const int i, const int j, double &x, double &y) const
{
    double widthScalar = _LogicalWidth / double(_ScreenWidth);
    double heightScalar = _LogicalHeight / double(_ScreenHeight);

    x = (i * widthScalar) - (_LogicalWidth / 2.0);
    y = -((j * heightScalar) - (_LogicalHeight / 2.0));
}

unsigned char *RTEngine::Screen(void) const
{
    return _Screen.get();
}

template <class Strategy>
void RTEngine::Render(void)
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

template void RTEngine::Render<RecursiveTraceStrategy>(void);
template void RTEngine::Render<IterativeTraceStrategy>(void);

// void RTEngine::CalculateSceneAsync()
// {
// 	//calculate ideal window sizes
// 	//
// 	DefaultWindowSizeI = SCREENWIDTH  / (int)(sqrt( double(MAXASYNCOPS) ));
// 	DefaultWindowSizeJ = SCREENHEIGHT / (int)(sqrt( double(MAXASYNCOPS) ));
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

    if(width < height)
    {
        double aspectRatio = double(height) / double(width);
        _LogicalWidth = LogicalSpaceSize;
        _LogicalHeight = aspectRatio * double(_LogicalWidth);
    }
    else
    {
        double aspectRatio = double(width) / double(height);
        _LogicalHeight = LogicalSpaceSize;
        _LogicalWidth = aspectRatio * double(_LogicalHeight);
    }
}
