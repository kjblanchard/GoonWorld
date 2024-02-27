#pragma once
#include <chrono>
namespace GoonWorld
{
    class TimeSpan
    {
    public:
        // Constructor taking duration in seconds
        TimeSpan(double seconds) : _durationSeconds(std::chrono::duration<double>(seconds)) {}

        inline double GetSeconds() const
        {
            return _durationSeconds.count();
        }

        inline long long GetMilliseconds() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(_durationSeconds).count();
        }

    private:
        std::chrono::duration<double> _durationSeconds;
    };

}
