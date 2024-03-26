#pragma once
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class TiledObject;
    class Player;
    class Fireflower : public GameObject, public ITakeDamage
    {
    public:
        Fireflower(TiledMap::TiledObject &object);
        Fireflower(geRectangle *rect);
        void Push();
        // void Update() override;
        void TakeDamage() override;
        static void TweenEndCallback(void *args);

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
    };

}