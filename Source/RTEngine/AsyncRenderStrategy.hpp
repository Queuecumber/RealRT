#include <array>
#include <thread>
#include <mutex>
#include <algorithm>

namespace RealRT
{
    template <int NumThreads>
    class AsyncRenderStrategy
    {
    public:
        AsyncRenderStrategy(int width, int height)
            : _Width(width), _Height(height), _NextScreenX(0), _NextScreenY(0), _Exit(false)
        {

        }

        void operator ()(const std::function<void (int, int)> &doTracing)
        {
            std::array<std::thread, NumThreads> taskFutures;

            std::generate(taskFutures.begin(), taskFutures.end(), [&doTracing, this]()
            {
                return std::thread([&doTracing, this]()
                {
                    while(!_Exit)
                    {
                        int i, j;

                        // Critical Section
                        {
                            std::lock_guard<std::mutex> lock(_IterationMutex);

                            if(_Exit)
                                continue;

                            i = _NextScreenX;
                            j = _NextScreenY;

                            if(i < (_Width - 1))
                            {
                                _NextScreenX++;
                            }
                            else
                            {
                                _NextScreenX = 0;

                                if(j < (_Height - 1))
                                {
                                    _NextScreenY++;
                                }
                                else
                                {
                                    _Exit = true;
                                }
                            }
                        }

                        doTracing(i, j);
                    }
                });
            });

            for(std::thread &f : taskFutures)
            {
                f.join();
            }
        }

    private:
        int _Width;
        int _Height;

        std::mutex _IterationMutex;
        int _NextScreenX;
        int _NextScreenY;
        bool _Exit;
    };
}
