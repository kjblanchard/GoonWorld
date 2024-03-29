#include <GoonWorld/tween/Tween.hpp>
#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>
using namespace GoonWorld;

template <>
void Tween<int>::Interpolate(double &deltaTimeSeconds)
{
    _currentTime += deltaTimeSeconds;
    if (_currentTime > _endTime)
    {
        // TODO this should be done different,
        // ie if it is looping we shouldn't just do this
        _currentTime = _endTime;
        _completed = true;
    }
    // Apply the easing function to the progress
    double easedProgress = GetProgressPercent();
    // Interpolate between start and end values based on the eased progress
    (*_value) = _start + static_cast<int>((_end - _start) * easedProgress);
    if (_completed && _callback && _args)
        _callback(_args);
}

template <>
void Tween<float>::Interpolate(double &deltaTimeSeconds)
{
}

template <>
void Tween<double>::Interpolate(double &deltaTimeSeconds)
{
}

template <>
void Tween<Point>::Interpolate(double &deltaTimeSeconds)
{
}

template <>
void Tween<geRectangle>::Interpolate(double &deltaTimeSeconds)
{
}