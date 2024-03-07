#pragma once
#include <GoonWorld/base/GameObject.hpp>
typedef struct gpOverlap gpOverlap;
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
        bool _movingRight, _isDead;
        void GoombaStaticBodyOverlap(gpOverlap* overlap);
        void GoombaMarioOverlap(gpOverlap* overlap);
        bool _shouldWalkAnim;
    };
}