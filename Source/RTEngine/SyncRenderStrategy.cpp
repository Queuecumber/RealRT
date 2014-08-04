#include "SyncRenderStrategy.hpp"

using namespace RealRT;

SyncRenderStrategy::SyncRenderStrategy(int width, int height)
    : _Width(width), _Height(height)
{

}

void SyncRenderStrategy::operator ()(const std::function<void(int, int)> &doTracing) const
{
    for(int j = 0; j < _Height; j++)
    {
        for(int i = 0; i < _Width; i++)
        {
            doTracing(i, j);
        }
    }
}
