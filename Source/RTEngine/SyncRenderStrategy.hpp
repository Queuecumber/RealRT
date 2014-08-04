#include <functional>

namespace RealRT
{
    class SyncRenderStrategy
    {
    public:
        SyncRenderStrategy(int width, int height);
        void operator ()(const std::function<void(int, int)> &doTracing) const;

    private:
        int _Width;
        int _Height;
    };
}
