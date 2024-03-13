#include <GoonWorld/core/Timer.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;

Timer::Timer(GameObject *owner, float waitTime, std::function<void(GameObject *)> callback)
    : _waitTime(waitTime), _owner(owner), _callback(callback)
{
}

bool Timer::Tick(float deltaTime)
{
    _currentWaitTime += deltaTime;
    if (_currentWaitTime >= _waitTime)
    {
        _callback(_owner);
        return true;
    }
    return false;
}