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
        ~Player() = default;
        void TakeDamage() override;
        void Update() override;

    private:
        struct PlayerFlags
        {
            static const int RunningButtonDown = 1 << 0;
            static const int CanJump = 1 << 1;
            static const int EnemyJustKilled = 1 << 2;
            static const int NoDeathVelocity = 1 << 3;
            static const int IsBig = 1 << 4;
            static const int IsInvincible = 1 << 5;
        };
        int32_t _playerFlags = 0;

        // Animations
    private:
        bool _shouldFallAnim, _shouldTurnAnim, _shouldRunAnim, _shouldIdleAnim;

    private:
        bool _isJumping = false, _isTurning = false, _isDying = false, _isDead = false;
        bool _isTurningBig = false, _isWinning = false, _isWinWalking = false;
        int _currentBigIterations = 0;
        int _coinsCollected = 0;

        // Config
    private:
        int *_jumpFrameVelocity, *_initialJumpVelocity, *_runSpeedBoost, *_walkSpeedBoost, *_maxRunSpeed, *_maxWalkSpeed, *_initialMoveVelocity;
        float *_maxJumpTime;

        // Timers
    private:
        float _currentJumpTime = 0, _enemyKillTime = 0, _currentDeadTime = 0, _currentBigIterationTime = 0, _currentInvincibleTime = 0;

        // Constants
    private:
        const float _winningWhistleTimer = .75;
        const float _deadTimer = 0.65;
        const float _invincibleTime = 1.0;
        const int _bigIterations = 4;
        const float _bigIterationTime = 0.1;

        // Components
    private:
        DebugDrawComponent *_debugDrawComponent = nullptr;
        PlayerInputComponent *_playerInputComponent = nullptr;
        RigidbodyComponent *_rigidbodyComponent = nullptr;
        AnimationComponent *_animationComponent = nullptr;

    private:
        void BindOverlapFunctions();
        void InvincibleTick();
        void GettingBigUpdate();
        void SlideFunc();
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
        void Powerup(bool isGettingBig);
        void Die();
        void Win();
        void WinWalking();

        // Overlap Functions
    private:
        static void ItemBoxOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void GoombaOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void MushroomOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void DeathBoxOverlap(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void WinBoxOverlap(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void CoinOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
    };
}