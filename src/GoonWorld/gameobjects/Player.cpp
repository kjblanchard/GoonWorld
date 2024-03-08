#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Sound.h>
#include <SDL2/SDL_rect.h>

using namespace GoonWorld;

static Sfx *jumpSound;

Player::Player(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _playerInputComponent = new PlayerInputComponent(0);
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(1);
    _animationComponent = new AnimationComponent("mario");
    jumpSound = (Sfx *)Content::LoadContent(ContentTypes::Sfx, "jump");

    _animationComponent->SizeMultiplier = 2;
    AddComponent({_debugDrawComponent, _playerInputComponent, _rigidbodyComponent, _animationComponent});
    bodyOverlapArgs args{1, 2, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                         {
                             Player *playerInstance = static_cast<Player *>(args);
                             playerInstance->GoombaOverlapFunc(overlapBody, overlap);
                         }};
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, args);
    CreateAnimationTransitions();
    InitializePlayerConfig();
}

void Player::InitializePlayerConfig()
{
    auto &playerConfig = Game::Instance()->GameSettings->PlayerConfigs;
    _jumpFrameVelocity = playerConfig.FrameJumpAcceleration;
    _initialJumpVelocity = playerConfig.InitialJumpVelocity;
    _runSpeedBoost = playerConfig.RunSpeedBoost;
    _walkSpeedBoost = playerConfig.WalkSpeedBoost;
    _maxWalkSpeed = playerConfig.MaxWalkSpeed;
    _maxRunSpeed = playerConfig.MaxRunSpeed;
    _maxJumpTime = playerConfig.MaxJumpTime;
    _initialMoveVelocity = playerConfig.InitialMoveVelocity;
}
void Player::Update()
{
    if (_enemyJustKilled)
    {
        _goombaKillTime += DeltaTime.GetTotalSeconds();
        if (_goombaKillTime > 0.3)
        {
            _enemyJustKilled = false;
            _goombaKillTime = 0;
        }
    }
    if (_goombaKillTime)
    {
        printf("Kill time: %f\n", _goombaKillTime);
    }

    _isJumping = _rigidbodyComponent->IsOnGround() ? false : _isJumping;

    _canJump = _rigidbodyComponent->IsOnGround();
    HandleInput();
    _isTurning = CheckIsTurning();

    // Turn faster.
    if (_isTurning)
    {
        _rigidbodyComponent->Acceleration().x *= 2;
    }
    // Do not control in the air so well in the opposite direction.
    if (!_rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Acceleration().x != 0)
    {
        if ((_rigidbodyComponent->Velocity().x > 0 && _rigidbodyComponent->Acceleration().x < 0) ||
            (_rigidbodyComponent->Velocity().x < 0 && _rigidbodyComponent->Acceleration().x > 0))
        {
            _rigidbodyComponent->Acceleration().x /= 3;
        }
    }
    AnimationUpdate();
    _animationComponent->Mirror = ShouldMirrorImage();
    _rigidbodyComponent->MaxVelocity().x = CalculateFrameMaxVelocity();
    LogInfo("Is jumping: %d, Can Jum %d", _isJumping, _canJump);

    GameObject::Update();
}

bool Player::CheckIsTurning()
{
    if (!_rigidbodyComponent->IsOnGround() || _rigidbodyComponent->Velocity().x == 0)
        return false;
    return ((_rigidbodyComponent->Velocity().x > 0 && _rigidbodyComponent->Acceleration().x < 0) ||
            (_rigidbodyComponent->Velocity().x < 0 && _rigidbodyComponent->Acceleration().x > 0));
}

bool Player::ShouldMirrorImage()
{
    if (_rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x != 0)
    {
        if (_shouldTurnAnim)
            return !(_rigidbodyComponent->Velocity().x < 0);
        return _rigidbodyComponent->Velocity().x < 0;
    }
    return _animationComponent->Mirror;
}

void Player::AnimationUpdate()
{
    if (_rigidbodyComponent->Velocity().x != 0 && _rigidbodyComponent->IsOnGround())
    {
        _animationComponent->AnimationSpeed = std::abs(_rigidbodyComponent->Velocity().x) / 100.0f;
    }
    _shouldFallAnim = _isJumping || !_rigidbodyComponent->IsOnGround();
    _shouldIdleAnim = _rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x == 0;
    _shouldRunAnim = _rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x != 0;
    _shouldTurnAnim = _isTurning;
}

float Player::CalculateFrameMaxVelocity()
{
    if (_isRunning)
        return _maxRunSpeed;
    if (std::abs(_rigidbodyComponent->Velocity().x) > _maxWalkSpeed)
        return _maxRunSpeed;
    return _maxWalkSpeed;
}

void Player::HandleInput()
{
    _isRunning = _playerInputComponent->IsButtonDownOrHeld(GameControllerButton::X);

    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        // If we are not moving, get a boost
        if (_rigidbodyComponent->Velocity().x == 0)
            _rigidbodyComponent->Acceleration().x -= _initialMoveVelocity;
        // If we are running or in the air?
        else if (_isRunning || !_rigidbodyComponent->IsOnGround())
        {
            auto moveSpeed = _runSpeedBoost;
            // LogInfo("Movespeed is %d", moveSpeed);
            _rigidbodyComponent->Acceleration().x -= moveSpeed * DeltaTime.GetTotalSeconds();
        }
        else
        {
            auto moveSpeed = _rigidbodyComponent->Velocity().x < -_maxWalkSpeed ? 0 : _walkSpeedBoost;
            // LogInfo("Movespeed is %d", moveSpeed);
            _rigidbodyComponent->Acceleration().x -= moveSpeed * DeltaTime.GetTotalSeconds();
        }
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        if (_rigidbodyComponent->Velocity().x == 0)
            _rigidbodyComponent->Acceleration().x += _initialMoveVelocity;
        else
        {
            auto moveSpeed = _isRunning ? _runSpeedBoost : _walkSpeedBoost;
            _rigidbodyComponent->Acceleration().x += moveSpeed * DeltaTime.GetTotalSeconds();
        }
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::A))
    {
        Jump();
    }
    else if (_playerInputComponent->IsButtonReleased(GameControllerButton::A))
    {
        _isJumping = false;
    }
}
void Player::CreateAnimationTransitions()
{
    _animationComponent->AddTransition("turn", "walk", false, &_shouldTurnAnim);
    _animationComponent->AddTransition("walk", "turn", true, &_shouldTurnAnim);
    _animationComponent->AddTransition("idle", "jump", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walk", "jump", true, &_shouldFallAnim);
    _animationComponent->AddTransition("jump", "idle", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("jump", "walk", true, &_shouldRunAnim);
    _animationComponent->AddTransition("idle", "walk", true, &_shouldRunAnim);
    _animationComponent->AddTransition("walk", "idle", true, &_shouldIdleAnim);
}
void Player::Jump()
{
    if (_isJumping)
    {
        if (_currentJumpTime < _maxJumpTime)
        {
            _rigidbodyComponent->Acceleration().y += (_jumpFrameVelocity * DeltaTime.GetTotalSeconds());
            _currentJumpTime += (float)DeltaTime.GetTotalSeconds();
        }
        else
        {
            _isJumping = _canJump = false;
        }
    }
    else if (_canJump)
    {
        _currentJumpTime = 0;
        _isJumping = true;
        _canJump = false;
        _rigidbodyComponent->Velocity().y = _initialJumpVelocity;
        PlaySfxOneShot(jumpSound, 1.0f);
    }
}
void Player::GoombaOverlapFunc(gpBody *overlapBody, gpOverlap *overlap)
{
    Goomba *goomba = (Goomba *)overlapBody->funcArgs;
    if (goomba->IsDead())
        return;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapDown && !_goombaKillTime)
    {
        _goombaKillTime += DeltaTime.GetTotalSeconds();
        _rigidbodyComponent->Velocity().y = _initialJumpVelocity;
        _currentJumpTime = 0;
        _isJumping = true;
        _enemyJustKilled = true;
        goomba->TakeDamage();
        return;
    }
    puts("Player should die");
}
Player::~Player()
{
}