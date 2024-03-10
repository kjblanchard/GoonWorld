#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/animation/Animation.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/Sound.h>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/common/Helpers.hpp>
#include <SDL2/SDL_rect.h>
using namespace GoonWorld;

static Sfx *jumpSound;

Player::Player(TiledMap::TiledObject &object)
    : _isDead(false), _isDying(false)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _playerInputComponent = new PlayerInputComponent(0);
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(1);
    _animationComponent = new AnimationComponent("mario", Point{0, -36});
    jumpSound = (Sfx *)Content::LoadContent(ContentTypes::Sfx, "jump");

    _animationComponent->SizeMultiplier = 2;
    AddComponent({_debugDrawComponent, _playerInputComponent, _rigidbodyComponent, _animationComponent});
    // _debugDrawComponent->Enabled(false);
    bodyOverlapArgs args{1, 2, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                         {
                             Player *playerInstance = static_cast<Player *>(args);
                             playerInstance->GoombaOverlapFunc(overlapBody, overlap);
                         }};
    bodyOverlapArgs brickArgs{1, (int)BodyTypes::ItemBrick, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                              {
                                  Player *playerInstance = static_cast<Player *>(args);
                                  playerInstance->ItemBoxOverlapFunc(overlapBody, overlap);
                              }};
    bodyOverlapArgs mushroomArgs{1, (int)BodyTypes::Mushroom, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                                 {
                                     Player *playerInstance = static_cast<Player *>(args);
                                     playerInstance->MushroomOverlapFunc(overlapBody, overlap);
                                 }};
    bodyOverlapArgs deathBoxArgs{1, (int)BodyTypes::DeathBox, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                                 {
                                     Player *playerInstance = static_cast<Player *>(args);
                                     if (playerInstance->_isDead || playerInstance->_isDying)
                                         return;
                                     playerInstance->_noDeathVelocity = true;
                                     playerInstance->TakeDamage();
                                 }};
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, args);
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, brickArgs);
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, mushroomArgs);
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, deathBoxArgs);
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
    if (_isTurningBig)
    {
        Powerup();
        return;
    }
    if (_enemyJustKilled)
        EnemyKilledTick();
    if (_isDying)
    {
        if (_currentDeadTime < _deadTimer)
        {
            _currentDeadTime += DeltaTime.GetTotalSeconds();
            _rigidbodyComponent->Velocity().y = 0;
        }
        else
        {
            if (!_noDeathVelocity)
                _rigidbodyComponent->Velocity().y = -250;
            _isDying = false;
            _isDead = true;
        }
    }
    _isJumping = _rigidbodyComponent->IsOnGround() ? false : _isJumping;
    _canJump = _rigidbodyComponent->IsOnGround();
    HandleInput();
    // _isTurning = CheckIsTurning();
    TurnPhysics();
    AirPhysics();
    AnimationUpdate();
    _animationComponent->Mirror = ShouldMirrorImage();
    _rigidbodyComponent->MaxVelocity().x = CalculateFrameMaxVelocity();
    LogInfo("Velocity %f:%f\nPos %f:%f", _rigidbodyComponent->Velocity().x, _rigidbodyComponent->Velocity().y, _rigidbodyComponent->_body->boundingBox.x, _rigidbodyComponent->_body->boundingBox.y);
    GameObject::Update();
}
void Player::EnemyKilledTick()
{
    _goombaKillTime += DeltaTime.GetTotalSeconds();
    if (_goombaKillTime > 0.3)
    {
        _enemyJustKilled = false;
        _goombaKillTime = 0;
    }
}

void Player::TurnPhysics()
{
    if (_isTurning)
        _rigidbodyComponent->Acceleration().x *= 2;
}

void Player::AirPhysics()
{
    if (!_rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Acceleration().x != 0)
    {
        if ((_rigidbodyComponent->Velocity().x > 0 && _rigidbodyComponent->Acceleration().x < 0) ||
            (_rigidbodyComponent->Velocity().x < 0 && _rigidbodyComponent->Acceleration().x > 0))
        {
            _rigidbodyComponent->Acceleration().x /= 3;
        }
    }
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
    if (_isRunningButtonDown)
        return _maxRunSpeed;
    if (std::abs(_rigidbodyComponent->Velocity().x) > _maxWalkSpeed)
        return _maxRunSpeed;
    return _maxWalkSpeed;
}

void Player::HandleInput()
{
    if (_isDead || _isDying || _isTurningBig)
        return;
    _isRunningButtonDown = _playerInputComponent->IsButtonDownOrHeld(GameControllerButton::X);

    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        HandleLeftRightMovement(false);
    }

    else if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        HandleLeftRightMovement(true);
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

void Player::HandleLeftRightMovement(bool movingRight)
{
    auto moveDirectionMultiplier = movingRight ? 1 : -1;
    // If we are not moving set initial velocity.
    if (_rigidbodyComponent->Velocity().x == 0)
    {
        _rigidbodyComponent->Acceleration().x += _initialMoveVelocity * moveDirectionMultiplier;
        return;
    }

    else if (_isRunningButtonDown)
    {
        auto moveSpeed = _runSpeedBoost * moveDirectionMultiplier;
        _rigidbodyComponent->Acceleration().x += moveSpeed * DeltaTime.GetTotalSeconds();
    }

    else
    {
        // TODO this is causing an issue where you actually can't turn if you are hitting this, need to expand
        // Check to see if we are walking but moving faster than our walk speed, if so no movespeed boost
        auto moveSpeed = std::abs(_rigidbodyComponent->Velocity().x) > _maxWalkSpeed ? 0 : _walkSpeedBoost * moveDirectionMultiplier;
        _rigidbodyComponent->Acceleration().x += moveSpeed * DeltaTime.GetTotalSeconds();
    }

    if (!_rigidbodyComponent->IsOnGround() || _rigidbodyComponent->Velocity().x == 0)
        _isTurning = false;
    else
    {
        _isTurning = ((_rigidbodyComponent->Velocity().x > 0 && _rigidbodyComponent->Acceleration().x < 0) ||
                      (_rigidbodyComponent->Velocity().x < 0 && _rigidbodyComponent->Acceleration().x > 0));
    }
}

void Player::CreateAnimationTransitions()
{
    // Small
    _animationComponent->AddTransition("turn", "idle", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("turn", "walk", false, &_shouldTurnAnim);
    _animationComponent->AddTransition("turn", "jump", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walk", "turn", true, &_shouldTurnAnim);
    _animationComponent->AddTransition("idle", "jump", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walk", "jump", true, &_shouldFallAnim);
    _animationComponent->AddTransition("jump", "idle", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("jump", "walk", true, &_shouldRunAnim);
    _animationComponent->AddTransition("idle", "walk", true, &_shouldRunAnim);
    _animationComponent->AddTransition("walk", "idle", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("walk", "dead", true, &_isDying);
    _animationComponent->AddTransition("idle", "dead", true, &_isDying);
    _animationComponent->AddTransition("turn", "dead", true, &_isDying);
    _animationComponent->AddTransition("jump", "dead", true, &_isDying);
    // Big
    _animationComponent->AddTransition("turnb", "idleb", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("turnb", "walkb", false, &_shouldTurnAnim);
    _animationComponent->AddTransition("turnb", "jumpb", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walkb", "turnb", true, &_shouldTurnAnim);
    _animationComponent->AddTransition("idleb", "jumpb", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walkb", "jumpb", true, &_shouldFallAnim);
    _animationComponent->AddTransition("jumpb", "idleb", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("jumpb", "walkb", true, &_shouldRunAnim);
    _animationComponent->AddTransition("idleb", "walkb", true, &_shouldRunAnim);
    _animationComponent->AddTransition("walkb", "idleb", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("walkb", "dead", true, &_isDying);
    _animationComponent->AddTransition("idleb", "dead", true, &_isDying);
    _animationComponent->AddTransition("turnb", "dead", true, &_isDying);
    _animationComponent->AddTransition("jumpb", "dead", true, &_isDying);
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
        gsPlaySfxOneShot(jumpSound, 1.0f);
    }
}
void Player::GoombaOverlapFunc(gpBody *overlapBody, gpOverlap *overlap)
{
    if (_isDead || _isDying)
        return;
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
        _noDeathVelocity = false;
        goomba->TakeDamage();
        return;
    }
    TakeDamage();
}
void Player::TakeDamage()
{
    Game::Instance()->PlayerDie(this);
    Game::Instance()->GetSound()->LoadBgm("dead");
    Game::Instance()->GetSound()->PlayBgm("dead");
    geSetPlayerLoops(0);

    _isDying = true;
    _rigidbodyComponent->SetCollidesWithStaticBody(false);
    _rigidbodyComponent->Velocity().x = 0;
    _rigidbodyComponent->Velocity().y = 0;
    _currentDeadTime = 0;
}

void Player::ItemBoxOverlapFunc(gpBody *overlapBody, gpOverlap *overlap)
{
    if (_isDead || _isDying)
        return;
    ItemBrick *itemBox = (ItemBrick *)overlapBody->funcArgs;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapUp)
    {
        itemBox->TakeDamage();
    }
}

void Player::MushroomOverlapFunc(gpBody *overlapBody, gpOverlap *overlap)
{
    if (_isDead || _isDying)
        return;
    Mushroom *mushroom = (Mushroom *)overlapBody->funcArgs;
    if (!mushroom->IsEnabled())
        return;
    Powerup();
    mushroom->TakeDamage();
}

void Player::Powerup()
{
    if (!_isTurningBig)
    {
        _isTurningBig = true;
        _currentBigIterations = 0;
        _currentBigIterationTime = 0;
        Game::Instance()->PlayerBig(this);
        // Should probably end and process nothing else this frame after this
    }

    // End
    if (_currentBigIterations > _bigIterations)
    {
        _isTurningBig = false;
        Game::Instance()->PlayerBig(nullptr);
        // _rigidbodyComponent->_body->boundingBox.y -= 26;
        auto newSize = Point{32, 64};
        _location.y -= 32;
        _rigidbodyComponent->_body->boundingBox.y -= 32;
        _rigidbodyComponent->SizeChange(newSize);
        _animationComponent->Offset(Point{0, -4});
        // _animationComponent->Offset(Point{0,-40});
        _debugDrawComponent->Size = newSize;
    }
    // Regular loop
    else
    {
        _currentBigIterationTime += DeltaTime.GetTotalSeconds();
        if (_currentBigIterationTime > _bigIterationTime)
        {
            _currentBigIterationTime -= _bigIterationTime;
            ++_currentBigIterations;

            auto currentAnim = _animationComponent->GetCurrentAnimation();
            auto newName = currentAnim.second->Name;
            auto isBig = endsWith(newName, "b");
            if (isBig)
            {
                newName.erase(newName.size() - 1);
                _animationComponent->Offset(Point{0, -36});
            }
            else
            {
                newName = newName + "b";
                _animationComponent->Offset(Point{0, -40});
            }
            _animationComponent->ChangeAnimation(newName);
        }
    }
}

Player::~Player()
{
}