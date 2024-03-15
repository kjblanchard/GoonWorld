

#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <queue>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

typedef struct gpBody gpBody;

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player;
    class Fireball : public GameObject, public ITakeDamage
    {
    public:
        Fireball(geRectangle *rect, std::queue<Fireball *> *home);
        void Update() override;
        void TakeDamage() override;
        void Push(Point spawnLocation, bool right);
        void End();

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        static void StaticBodyOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void GoombaOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        std::queue<Fireball *> *_fireballHome;
        bool _hitGround = false, _movingRight = true;
        const int _moveSpeed = 200;
        const int _jumpVelocity = -175;
    };

}