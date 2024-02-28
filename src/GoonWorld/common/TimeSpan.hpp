#pragma once
#include <chrono>
namespace GoonWorld
{
    class TimeSpan
    {
    public:
        // Constructor taking duration in ms
        TimeSpan(double seconds) : _durationSeconds(std::chrono::duration<double>(seconds)) {}

        inline double GetTotalSeconds() const
        {
            return _durationSeconds.count();
        }

        inline long long GetSeconds() const
        {
            return std::chrono::duration_cast<std::chrono::seconds>(_durationSeconds).count();
        }

        inline double GetTotalMilliseconds() const
        {
            return _durationSeconds.count() * 1000;
        }

        inline long long GetMilliseconds() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(_durationSeconds).count();
        }

    private:
        std::chrono::duration<double> _durationSeconds;
    };

}
