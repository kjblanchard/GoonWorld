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
        bool _isJumping = false, _canJump = false, _isTurning = false, _isRunningButtonDown = false, _enemyJustKilled = false, _isDying = false, _isDead = false, _noDeathVelocity = false, _isTurningBig = false, _isBig = false, _isInvincible = false;
        int _jumpFrameVelocity = 0, _initialJumpVelocity = 0, _runSpeedBoost = 0, _walkSpeedBoost = 0, _maxRunSpeed = 0, _maxWalkSpeed = 0, _initialMoveVelocity = 0, _currentBigIterations = 0;
        float _currentJumpTime = 0, _maxJumpTime = 0, _goombaKillTime = 0, _currentDeadTime = 0, _currentBigIterationTime = 0, _currentInvincibleTime = 0;
        const float _deadTimer = 0.65;
        const float _invincibleTime = 1.0;
        const int _bigIterations = 4;
        const float _bigIterationTime = 0.1;

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

        DebugDrawComponent *_debugDrawComponent = nullptr;
        PlayerInputComponent *_playerInputComponent = nullptr;
        RigidbodyComponent *_rigidbodyComponent = nullptr;
        AnimationComponent *_animationComponent = nullptr;

    private:
        void GoombaOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void ItemBoxOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void MushroomOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        void Powerup(bool isGettingBig);
        void Die();
        // friend void GoombaOverlapFuncCallback(void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
    };
}