#pragma once
namespace GoonWorld
{
    class GameObject;
    class Timer
    {
    public:
        Timer(GameObject *owner, float waitTime, std::function<bool(GameObject *obj, bool isFinished)> callback);
        inline void Reset() { _currentWaitTime = 0; }
        bool Tick(float deltaTime);

    private:
        float _waitTime;
        float _currentWaitTime;
        GameObject *_owner;
        std::function<bool(GameObject *, bool)> _callback;
    };
}