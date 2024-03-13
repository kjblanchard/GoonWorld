#pragma once
namespace GoonWorld
{
    class GameObject;
    class Timer
    {
    public:
        Timer(GameObject *owner, float waitTime, std::function<void(GameObject *)> callback);
        bool Tick(float deltaTime);

    private:
        float _waitTime;
        float _currentWaitTime;
        GameObject *_owner;
        std::function<void(GameObject *)> _callback;
    };
}