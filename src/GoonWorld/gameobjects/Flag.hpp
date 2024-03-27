#pragma once
#include <memory>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class RigidbodyComponent;
    class AnimationComponent;
    class TiledObject;
    struct Observer;
    struct Event;
    class Flag : public GameObject, public ITakeDamage
    {
    public:
        Flag(TiledMap::TiledObject &object);
        void TakeDamage() override;

    private:
        void PlayerWinEvent(Event &event);
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        std::unique_ptr<Observer> _playerWinObserver;
    };

}