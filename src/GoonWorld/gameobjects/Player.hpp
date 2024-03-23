#pragma once
#include <queue>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>
#include <GoonWorld/models/AppSettings.hpp>

typedef struct gpBody gpBody;
typedef struct gpOverlap gpOverlap;

namespace GoonWorld
{
    class Fireball;
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class BoxColliderComponent;

    class Player : public GameObject,
                   public ITakeDamage
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
            static const int DeadNoGravity = 1 << 3;
            static const int IsBig = 1 << 4;
            static const int IsInvincible = 1 << 5;
            static const int IsSuper = 1 << 6;
            static const int IsThrowingFireball = 1 << 7;
        };
        int32_t _playerFlags = 0;
        AppSettings::PlayerConfig *_playerConfig;
        int _currentBigIterations = 0;
        int _coinsCollected = 0;
        std::queue<Fireball *> _fireballs;
        // Animations
    private:
        bool _shouldFallAnim, _shouldTurnAnim, _shouldRunAnim, _shouldIdleAnim;
        bool _shouldThrowFireballIdleAnim = false, _shouldThrowFireballRunAnim = false;
        bool _shouldClimbAnim = false;
        // State
    private:
        bool _isJumping = false, _isTurning = false, _isDying = false, _isDead = false;
        bool _isTurningBig = false, _isWinning = false, _isWinWalking = false, _isOnGround = false;
        // Timers
    private:
        float _currentJumpTime = 0, _currentEnemyKillTime = 0, _currentDeadTime = 0, _currentBigIterationTime = 0, _currentInvincibleTime = 0;
        float _currentFireballTimer = 0;
        // Constants
    private:
        const float _winningWhistleTimer = .25;
        const float _deadTimer = 0.65;
        const float _invincibleTime = 1.0;
        const int _bigIterations = 4;
        const float _bigIterationTime = 0.1;
        const float _fireballThrowTime = 0.15;
        // Components
    private:
        DebugDrawComponent *_debugDrawComponent = nullptr;
        PlayerInputComponent *_playerInputComponent = nullptr;
        RigidbodyComponent *_rigidbodyComponent = nullptr;
        AnimationComponent *_animationComponent = nullptr;
        BoxColliderComponent* _boxColliderComponent = nullptr;

    private:
        void BindOverlapFunctions();
        void ShootFireball();
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
        void JumpExtend();
        float CalculateFrameMaxVelocity();
        void PowerChangeStart(bool isGettingBig);
        void SuperPowerupTick();
        void PowerupTick();
        void Die();
        void Win();
        void WinWalking();

        // Overlap Functions
    private:
        static void BrickOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void ItemBoxOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void GoombaOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void GoombaOverlapFuncJumpBox(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void MushroomOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void FireflowerOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void DeathBoxOverlap(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void WinBoxOverlap(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void CoinOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void FlagOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
        static void EndLevelStaticOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
    };
}