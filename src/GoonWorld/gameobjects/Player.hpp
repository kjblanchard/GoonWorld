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

        bool _shouldRunAnim = false;
        bool _shouldIdleAnim = false;
        // Jump
    private:
        bool _isJumping, _canJump;
        int _jumpVelocity = 5;
        float _currentJumpTime, _maxJumpTime = 0.25f;

    private:
        void CreateAnimationTransitions();
        void Jump();
        DebugDrawComponent *_debugDrawComponent;
        PlayerInputComponent *_playerInputComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent;
    };
}