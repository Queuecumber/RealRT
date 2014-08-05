#include "RTEngine.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include <algorithm>

using namespace RealRT;

RTEngine::RTEngine(int width,int height)
{
	Resize(width, height);
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

void RTEngine::Resize(int width, int height)
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
