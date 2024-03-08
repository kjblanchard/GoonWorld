#pragma once
#include <GoonWorld/base/GameObject.hpp>

typedef struct gpBody gpBody;
typedef struct gpOverlap gpOverlap;

namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Player : public GameObject
    {
    public:
        Player(TiledMap::TiledObject &object);
        inline bool CanDamage() { return _enemyJustKilled == false; }
        void Update() override;

        ~Player();

    private:
        bool _shouldFallAnim = false;
        bool _shouldTurnAnim = false;
        bool _shouldRunAnim = false;
        bool _shouldIdleAnim = false;

    private:
        bool _isJumping, _canJump, _isTurning, _isRunning, _enemyJustKilled = false;
        int _jumpFrameVelocity, _initialJumpVelocity, _runSpeedBoost, _walkSpeedBoost, _maxRunSpeed, _maxWalkSpeed, _initialMoveVelocity;
        float _currentJumpTime, _maxJumpTime, _goombaKillTime;

    private:
        void HandleInput();
        void AnimationUpdate();
        bool ShouldMirrorImage();
        bool CheckIsTurning();
        void InitializePlayerConfig();
        void CreateAnimationTransitions();
        void Jump();
        float CalculateFrameMaxVelocity();

        DebugDrawComponent *_debugDrawComponent;
        PlayerInputComponent *_playerInputComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;

    private:
        void GoombaOverlapFunc(gpBody *overlapBody, gpOverlap *overlap);
        // friend void GoombaOverlapFuncCallback(void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap);
    };
}