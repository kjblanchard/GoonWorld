#pragma once
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

typedef struct gpBody gpBody;
typedef struct gpOverlap gpOverlap;

namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player : public GameObject, public ITakeDamage
    {
    public:
        Player(TiledMap::TiledObject &object);
        inline bool CanDamage() { return _enemyJustKilled == false; }
        void TakeDamage() override;
        void Update() override;

        ~Player();

    private:
        bool _shouldFallAnim, _shouldTurnAnim, _shouldRunAnim, _shouldIdleAnim;

    private:
        bool _isJumping, _canJump, _isTurning, _isRunningButtonDown, _enemyJustKilled, _isDying, _isDead, _noDeathVelocity;
        int _jumpFrameVelocity, _initialJumpVelocity, _runSpeedBoost, _walkSpeedBoost, _maxRunSpeed, _maxWalkSpeed, _initialMoveVelocity;
        float _currentJumpTime, _maxJumpTime, _goombaKillTime, _currentDeadTime;
        const float _deadTimer = 0.50;

    private:
        void HandleInput();
        void AnimationUpdate();
        bool ShouldMirrorImage();
        void EnemyKilledTick();
        void TurnPhysics();
        void AirPhysics();
        void InitializePlayerConfig();
        void CreateAnimationTransitions();
        void HandleLeftRightMovement(bool movingRight);
        void Jump();
        float CalculateFrameMaxVelocity();

        DebugDrawComponent *_debugDrawComponent;
        PlayerInputComponent *_playerInputComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;

    private:
        void GoombaOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void ItemBoxOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void MushroomOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void Powerup();
        // friend void GoombaOverlapFuncCallback(void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
    };
}