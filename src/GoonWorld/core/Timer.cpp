#include <GoonWorld/core/Timer.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;

Timer::Timer(GameObject *owner, float waitTime, std::function<bool(GameObject *, bool)> callback)
    : _waitTime(waitTime), _owner(owner), _callback(callback)
{
}

bool Timer::Tick(float deltaTime)
{
    _currentWaitTime += deltaTime;
    auto isComplete = _currentWaitTime >= _waitTime;
    auto callbackResponse = _callback(_owner, isComplete);
    return isComplete && callbackResponse;
}