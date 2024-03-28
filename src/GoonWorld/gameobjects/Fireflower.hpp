#pragma once
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class RigidbodyComponent;
    class AnimationComponent;
    class TiledObject;
    class Fireflower : public GameObject, public ITakeDamage
    {
    public:
        Fireflower(TiledMap::TiledObject &object);
        Fireflower(geRectangle *rect);
        void Push();
        void TakeDamage() override;
        static void TweenEndCallback(void *args);

    private:
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
    };

}