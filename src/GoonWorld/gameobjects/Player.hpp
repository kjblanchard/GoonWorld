#pragma once
#include <GoonWorld/base/GameObject.hpp>

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
        void Update() override;

        ~Player();

        // Animation
    private:
        bool _shouldFallAnim = false;
        bool _shouldTurnAnim = false;
        bool _shouldRunAnim = false;
        bool _shouldIdleAnim = false;
        // Jump
    private:
        bool _isJumping, _canJump, _isTurning, _isRunning;
        int _jumpFrameVelocity, _initialJumpVelocity, _runSpeedBoost, _walkSpeedBoost, _maxRunSpeed, _maxWalkSpeed, _initialMoveVelocity;
        float _currentJumpTime, _maxJumpTime;

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
    };
}