#pragma once
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class TiledObject;
    class Coin : public GameObject, public ITakeDamage
    {
    public:
        Coin(TiledMap::TiledObject &object);
        void TakeDamage() override;

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
    };

}