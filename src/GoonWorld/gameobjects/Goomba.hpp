#pragma once
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>
typedef struct gpOverlap gpOverlap;
namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player;
    class Goomba : public GameObject, ITakeDamage
    {
    public:
        Goomba(TiledMap::TiledObject &object);
        inline bool IsDead() { return _isDead; }
        void Update() override;
        void TakeDamage() override;
        void DamageGoomba(Player* player);
        ~Goomba();

    private:
        DebugDrawComponent *_debugDrawComponent = nullptr;
        RigidbodyComponent *_rigidbodyComponent = nullptr;
        AnimationComponent *_animationComponent = nullptr;
        bool _movingRight = false, _isDead = false;
        const float _deadTimer = 0.5f;
        float _currentDeadTime = 0.0f;
        void GoombaStaticBodyOverlap(gpOverlap *overlap);
        // void GoombaMarioOverlap(gpOverlap *overlap);
        bool _shouldWalkAnim = false;
    };
}