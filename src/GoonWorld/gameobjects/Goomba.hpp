#pragma once
#include <GoonWorld/base/GameObject.hpp>
typedef struct gpOverlap;
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
        void GoombaOverlap();
        void Update() override;
        ~Goomba();

    private:

        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        bool _movingRight;
        void GoombaStaticBodyOverlap(gpOverlap* overlap);
        bool _shouldWalkAnim;
    };
}