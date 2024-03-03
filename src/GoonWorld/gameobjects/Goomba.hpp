#pragma once
#include <GoonWorld/base/GameObject.hpp>
namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Goomba : public GameObject
    {
    public:
        Goomba(TiledMap::TiledObject &object);
        void Update() override;
        ~Goomba();

    private:

        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        bool _shouldWalkAnim;
    };
}