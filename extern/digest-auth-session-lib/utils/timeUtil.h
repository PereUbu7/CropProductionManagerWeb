#include <chrono>

template<typename resolution>
unsigned long getTimeSinceEpoch()
{
    std::chrono::time_point<std::chrono::system_clock> now = 
    std::chrono::system_clock::now();
 
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<resolution>(duration).count();

    return millis;
}