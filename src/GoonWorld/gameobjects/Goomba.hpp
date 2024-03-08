#pragma once
#include <GoonWorld/base/GameObject.hpp>
typedef struct gpOverlap gpOverlap;
namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player;
    class Goomba : public GameObject
    {
    public:
        Goomba(TiledMap::TiledObject &object);
        inline bool IsDead() { return _isDead; }
        void Update() override;
        void DamageGoomba(Player* player);
        ~Goomba();

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
        bool _movingRight, _isDead = false;
        const float _deadTimer = 0.5f;
        float _currentDeadTime = 0.0f;
        void GoombaStaticBodyOverlap(gpOverlap *overlap);
        void GoombaMarioOverlap(gpOverlap *overlap);
        bool _shouldWalkAnim;
    };
}