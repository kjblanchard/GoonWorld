#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/BoxColliderComponent.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/animation/Animation.hpp>
#include <GoonWorld/gameobjects/Coin.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonWorld/gameobjects/Fireflower.hpp>
#include <GoonWorld/gameobjects/ItemBrick.hpp>
#include <GoonWorld/gameobjects/ItemBox.hpp>
#include <GoonWorld/gameobjects/Fireball.hpp>
#include <GoonWorld/gameobjects/Flag.hpp>
#include <GoonWorld/common/Helpers.hpp>
#include <GoonWorld/events/Event.hpp>
#include <GoonWorld/events/EventTypes.hpp>
#include <GoonWorld/core/Timer.hpp>
#include <GoonWorld/content/Bgm.hpp>
#include <GoonWorld/content/Sfx.hpp>
using namespace GoonWorld;

const char *jumpSound = "jump";
const char *powerDownSound = "powerdown";
const char *whistleSound = "whistle";
const char *playerDieBgm = "playerdie";
const char *playerWinBgm = "win";

static Sfx *jumpSfx;
static Sfx *powerDownSfx;
static Sfx *whistleSfx;

static Bgm *winBgm;
static Bgm *dieBgm;

Player::Player(TiledMap::TiledObject &object)
    : _playerConfig(&GetGame().GetAppSettings().PlayerConfigs), _isDying(false), _isDead(false)
{
    _location = Point{object.X, object.Y};
    _playerInputComponent = new PlayerInputComponent(0);
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    bodyRect.w += _playerConfig->RigidBodyOffsetW;
    bodyRect.h += _playerConfig->RigidBodyOffsetH;
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect, Point{_playerConfig->RigidBodyOffsetX, _playerConfig->RigidBodyOffsetY});
    _rigidbodyComponent->SetBodyType(1);
    _animationComponent = new AnimationComponent("mario", Point{0, -22});
    auto boxRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    boxRect.w += _playerConfig->JumpColliderOffsetW;
    boxRect.h += _playerConfig->JumpColliderOffsetH;
    _boxColliderComponent = new BoxColliderComponent(&boxRect, Point{_playerConfig->JumpColliderOffsetX, _playerConfig->JumpColliderOffsetY});
    _boxColliderComponent->SetBodyType(1);
    _rigidbodyComponent->AddBoxCollider(_boxColliderComponent);
    jumpSfx = Sfx::SfxFactory(jumpSound);
    powerDownSfx = Sfx::SfxFactory(powerDownSound);
    whistleSfx = Sfx::SfxFactory(whistleSound);

    AddComponent({_boxColliderComponent, _playerInputComponent, _rigidbodyComponent, _animationComponent});
    _animationComponent->ChangeLayer(2);
    BindOverlapFunctions();
    CreateAnimationTransitions();
    InitializePlayerConfig();
    // GetGame().GetCamera()->SetFollowTarget(this);
    static int xLoc = 0;
    for (size_t i = 0; i < 4; i++)
    {
        auto rect = geRectangle{xLoc, 0, 16, 16};
        auto fireball = new Fireball(&rect, &_fireballs);
        _fireballs.push(fireball);
        xLoc += 32;
    }
    winBgm = Bgm::BgmFactory(playerWinBgm);
    dieBgm = Bgm::BgmFactory(playerDieBgm);

    // _rigidbodyComponent->SetDebug(true);
    // _boxColliderComponent->SetDebug(true);
}
void Player::BindOverlapFunctions()
{
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Goomba, &Player::GoombaOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::DeathBox, &Player::DeathBoxOverlap);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Mushroom, &Player::MushroomOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Fireflower, &Player::FireflowerOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::WinBox, &Player::WinBoxOverlap);

    _boxColliderComponent->AddOverlapFunction((int)BodyTypes::Goomba, &Player::GoombaOverlapFuncJumpBox);
    _boxColliderComponent->AddOverlapFunction((int)BodyTypes::Coin, &Player::CoinOverlapFunc);
    _boxColliderComponent->AddOverlapFunction((int)BodyTypes::ItemBrick, &Player::ItemBoxOverlapFunc);
    _boxColliderComponent->AddOverlapFunction((int)BodyTypes::ItemBox, &Player::ItemBoxOverlapFunc);
}

void Player::InitializePlayerConfig()
{
}
void Player::ShootFireball()
{
    if (!IsFlagSet(_playerFlags, PlayerFlags::IsSuper) || _fireballs.empty())
        return;
    _currentFireballTimer = 0;
    SetFlag(_playerFlags, PlayerFlags::IsThrowingFireball, true);
    auto location = _location;
    auto ballRight = !_animationComponent->Mirror;
    location.x += ballRight ? 8 : -8;
    _fireballs.front()->Push(location, ballRight);
    _fireballs.pop();
}

void Player::Update()
{
    if (_isTurningBig)
    {
        // Need to handle which
        if (IsFlagSet(_playerFlags, PlayerFlags::IsSuper))
        {
            SuperPowerupTick();
        }
        else
        {
            PowerupTick();
        }
        return;
    }
    if (_isWinning)
    {
        Win();
        return;
    }
    if (_isWinWalking)
    {
        WinWalking();
    }
    if (IsFlagSet(_playerFlags, PlayerFlags::EnemyJustKilled))
        EnemyKilledTick();
    if (IsFlagSet(_playerFlags, PlayerFlags::IsInvincible))
        InvincibleTick();
    if (_isDying)
    {
        if (_currentDeadTime < _deadTimer)
        {
            _currentDeadTime += DeltaTime.GetTotalSeconds();
            _rigidbodyComponent->Velocity().y = 0;
        }
        else
        {
            if (!IsFlagSet(_playerFlags, PlayerFlags::DeadNoGravity))
                _rigidbodyComponent->Velocity().y = -250;
            _isDying = false;
            _isDead = true;
            SetFlag(_playerFlags, PlayerFlags::CanExitLevel, true);
        }
    }
    if (IsFlagSet(_playerFlags, PlayerFlags::IsThrowingFireball))
    {
        _currentFireballTimer += DeltaTime.GetTotalSeconds();
        if (_currentFireballTimer >= _fireballThrowTime)
            SetFlag(_playerFlags, PlayerFlags::IsThrowingFireball, false);
    }
    if (_rigidbodyComponent->JustGotOnGround())
    {
        _isOnGround = true;
        _isJumping = false;
    }
    if (_rigidbodyComponent->JustLeftGround())
        _isOnGround = false;

    // _isJumping = _rigidbodyComponent->IsOnGround() ? false : _isJumping;
    SetFlag(_playerFlags, PlayerFlags::CanJump, _isOnGround);
    HandleInput();
    TurnPhysics();
    AirPhysics();
    AnimationUpdate();
    _animationComponent->Mirror = ShouldMirrorImage();
    _rigidbodyComponent->MaxVelocity().x = CalculateFrameMaxVelocity();
    // LogInfo("Currently jumping : %d", _isJumping);
    GameObject::Update();
}
void Player::InvincibleTick()
{
    _currentInvincibleTime += DeltaTime.GetTotalSeconds();
    int interval = static_cast<int>(_currentInvincibleTime / 0.1);
    bool isVisible = interval % 2 == 0;
    if (_currentInvincibleTime >= 0.9)
    {
        isVisible = true;
        SetFlag(_playerFlags, PlayerFlags::IsInvincible, false);
        _currentInvincibleTime = 0;
    }
    _animationComponent->Visible(isVisible);
}

void Player::EnemyKilledTick()
{
    _currentEnemyKillTime += DeltaTime.GetTotalSeconds();
    if (_currentEnemyKillTime > 0.3)
    {
        SetFlag(_playerFlags, PlayerFlags::EnemyJustKilled, false);
        _currentEnemyKillTime = 0;
    }
}

void Player::TurnPhysics()
{
    if (_isTurning)
        _rigidbodyComponent->Acceleration().x *= 2;
}

void Player::AirPhysics()
{
    if (!_isOnGround && _rigidbodyComponent->Acceleration().x != 0)
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
    if (_isOnGround && _rigidbodyComponent->Velocity().x != 0)
    {
        if (_shouldTurnAnim)
            return !(_rigidbodyComponent->Velocity().x < 0);
        return _rigidbodyComponent->Velocity().x < 0;
    }
    return _animationComponent->Mirror;
}

void Player::AnimationUpdate()
{
    if (_rigidbodyComponent->Velocity().x != 0 && _isOnGround)
    {
        _animationComponent->AnimationSpeed = std::abs(_rigidbodyComponent->Velocity().x) / 100.0f;
    }
    _shouldFallAnim = _isJumping || !_isOnGround;
    _shouldIdleAnim = _isOnGround && _rigidbodyComponent->Velocity().x == 0;
    _shouldRunAnim = _isOnGround && _rigidbodyComponent->Velocity().x != 0;
    _shouldTurnAnim = _isTurning;
    if (IsFlagSet(_playerFlags, PlayerFlags::IsThrowingFireball))
    {
        _shouldThrowFireballIdleAnim = _rigidbodyComponent->Velocity().x == 0 || !_rigidbodyComponent->IsOnGround();
        _shouldThrowFireballRunAnim = _rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x != 0;
    }
    else
    {
        _shouldThrowFireballIdleAnim = false;
        _shouldThrowFireballRunAnim = false;
    }
}

float Player::CalculateFrameMaxVelocity()
{
    if (IsFlagSet(_playerFlags, PlayerFlags::RunningButtonDown))
        // return *_maxRunSpeed;
        return _playerConfig->MaxRunSpeed;
    if (std::abs(_rigidbodyComponent->Velocity().x) > _playerConfig->MaxWalkSpeed)
        return _playerConfig->MaxRunSpeed;
    // return *_maxWalkSpeed;
    return _playerConfig->MaxWalkSpeed;
}

void Player::HandleInput()
{
    if (_isDying || _isTurningBig || _isWinning)
        return;
    if (_isDead || _isWinWalking)
    {
        if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::A))
        {
            if (!IsFlagSet(_playerFlags, PlayerFlags::CanExitLevel))
                return;
            if (_isDead)
                Game::Instance()->TriggerRestartLevel();
            else
                Game::Instance()->TriggerNextLevel();
        }
        return;
    }
    SetFlag(_playerFlags, PlayerFlags::RunningButtonDown, _playerInputComponent->IsButtonDownOrHeld(GameControllerButton::X));
    if (_playerInputComponent->IsButtonPressed(GameControllerButton::X))
    {
        ShootFireball();
    }

    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        HandleLeftRightMovement(false);
    }

    else if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        HandleLeftRightMovement(true);
    }
    if (_playerInputComponent->IsButtonPressed(GameControllerButton::A))
    {
        Jump();
    }

    if (_playerInputComponent->IsButtonDown(GameControllerButton::A))
    {
        JumpExtend();
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
        // _rigidbodyComponent->Velocity().x += *_initialMoveVelocity * moveDirectionMultiplier;
        _rigidbodyComponent->Velocity().x += _playerConfig->InitialMoveVelocity * moveDirectionMultiplier;
        return;
    }

    else if (IsFlagSet(_playerFlags, PlayerFlags::RunningButtonDown))
    {
        // auto moveSpeed = *_runSpeedBoost * moveDirectionMultiplier;
        auto moveSpeed = _playerConfig->RunSpeedBoost * moveDirectionMultiplier;
        _rigidbodyComponent->Acceleration().x += moveSpeed * DeltaTime.GetTotalSeconds();
    }

    else
    {
        auto moveSpeed = _playerConfig->WalkSpeedBoost * moveDirectionMultiplier;
        // You are walking but moving faster than your max walk speed, so you gain NOTHING
        if ((movingRight && _rigidbodyComponent->Velocity().x > _playerConfig->MaxWalkSpeed) ||
            (!movingRight && _rigidbodyComponent->Velocity().x < -_playerConfig->MaxWalkSpeed))
        {
            moveSpeed = 0;
        }
        _rigidbodyComponent->Acceleration().x += moveSpeed * DeltaTime.GetTotalSeconds();
    }

    if (!_isOnGround || _rigidbodyComponent->Velocity().x == 0)
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
    // Super
    _animationComponent->AddTransition("turnf", "idlef", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("turnf", "walkf", false, &_shouldTurnAnim);
    _animationComponent->AddTransition("turnf", "jumpf", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walkf", "turnf", true, &_shouldTurnAnim);
    _animationComponent->AddTransition("idlef", "jumpf", true, &_shouldFallAnim);
    _animationComponent->AddTransition("walkf", "jumpf", true, &_shouldFallAnim);
    _animationComponent->AddTransition("jumpf", "idlef", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("jumpf", "walkf", true, &_shouldRunAnim);
    _animationComponent->AddTransition("idlef", "walkf", true, &_shouldRunAnim);
    _animationComponent->AddTransition("walkf", "idlef", true, &_shouldIdleAnim);
    _animationComponent->AddTransition("walkf", "dead", true, &_isDying);
    _animationComponent->AddTransition("idlef", "dead", true, &_isDying);
    _animationComponent->AddTransition("turnf", "dead", true, &_isDying);
    _animationComponent->AddTransition("jumpf", "dead", true, &_isDying);
    // Fireball
    _animationComponent->AddTransition("walkf", "walkft", true, &_shouldThrowFireballRunAnim);
    _animationComponent->AddTransition("walkft", "walkf", false, &_shouldThrowFireballRunAnim);
    _animationComponent->AddTransition("idlef", "idleft", true, &_shouldThrowFireballIdleAnim);
    _animationComponent->AddTransition("idleft", "idlef", false, &_shouldThrowFireballIdleAnim);
    _animationComponent->AddTransition("idleft", "runft", true, &_shouldThrowFireballRunAnim);
    _animationComponent->AddTransition("runft", "idleft", true, &_shouldThrowFireballIdleAnim);
    _animationComponent->AddTransition("turnf", "walkft", true, &_shouldThrowFireballRunAnim);
    _animationComponent->AddTransition("jumpf", "idleft", true, &_shouldThrowFireballIdleAnim);
    // climbing
    _animationComponent->AddTransition("walk", "climb", true, &_shouldClimbAnim);
    _animationComponent->AddTransition("walkb", "climbb", true, &_shouldClimbAnim);
    _animationComponent->AddTransition("walkf", "climbf", true, &_shouldClimbAnim);
    _animationComponent->AddTransition("climb", "walk", false, &_shouldClimbAnim);
    _animationComponent->AddTransition("climbb", "walkb", false, &_shouldClimbAnim);
    _animationComponent->AddTransition("climbf", "walkf", false, &_shouldClimbAnim);
    _animationComponent->AddTransition("jumpf", "climbf", true, &_shouldClimbAnim);
    _animationComponent->AddTransition("jumpb", "climbb", true, &_shouldClimbAnim);
    _animationComponent->AddTransition("jump", "climb", true, &_shouldClimbAnim);
}

void Player::Jump()
{
    if (!IsFlagSet(_playerFlags, PlayerFlags::CanJump))
        return;
    _currentJumpTime = 0;
    _isJumping = true;
    SetFlag(_playerFlags, PlayerFlags::CanJump, false);
    _rigidbodyComponent->Velocity().y = _playerConfig->InitialJumpVelocity;
    jumpSfx->Play();
}

void Player::JumpExtend()
{
    if (!_isJumping)
        return;
    if (_currentJumpTime >= _playerConfig->MaxJumpTime)
    {
        _isJumping = false;
        SetFlag(_playerFlags, PlayerFlags::CanJump, false);
        return;
    }
    _rigidbodyComponent->Acceleration().y += (_playerConfig->FrameJumpAcceleration * DeltaTime.GetTotalSeconds());
    _currentJumpTime += (float)DeltaTime.GetTotalSeconds();
}

void Player::GoombaOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto player = (Player *)instance;
    if (player->_isDead || player->_isDying || player->IsFlagSet(player->_playerFlags, PlayerFlags::EnemyJustKilled))
        return;
    Goomba *goomba = (Goomba *)((gpBody*) overlapBody)->funcArgs;
    if (goomba->IsDead())
        return;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapDown)
    {
        player->_currentEnemyKillTime = 0;
        // player->_rigidbodyComponent->Velocity().y = *player->_initialJumpVelocity;
        auto vel = player->_playerConfig->InitialJumpVelocity / 2;
        if (player->_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::A))
        {
            vel *= 2;
        }

        player->_rigidbodyComponent->Velocity().y = vel;
        player->_currentJumpTime = 0;
        player->_isJumping = true;
        player->SetFlag(player->_playerFlags, PlayerFlags::EnemyJustKilled, true);
        player->SetFlag(player->_playerFlags, PlayerFlags::DeadNoGravity, false);
        goomba->TakeDamage();
        return;
    }
    player->TakeDamage();
}

void Player::GoombaOverlapFuncJumpBox(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto player = (Player *)instance;
    if (player->_isDead || player->_isDying || player->IsFlagSet(player->_playerFlags, PlayerFlags::EnemyJustKilled))
        return;
    Goomba *goomba = (Goomba *)((gpBody*) overlapBody)->funcArgs;
    if (goomba->IsDead())
        return;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapDown)
        if (overlap->overlapDirection == gpOverlapDirections::gpOverlapDown)
        {
            player->_currentEnemyKillTime = 0;
            // player->_rigidbodyComponent->Velocity().y = *player->_initialJumpVelocity;
            auto vel = player->_playerConfig->InitialJumpVelocity / 2;
            if (player->_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::A))
            {
                vel *= 2;
            }

            player->_rigidbodyComponent->Velocity().y = vel;
            player->_currentJumpTime = 0;
            player->_isJumping = true;
            player->SetFlag(player->_playerFlags, PlayerFlags::EnemyJustKilled, true);
            player->SetFlag(player->_playerFlags, PlayerFlags::DeadNoGravity, false);
            goomba->TakeDamage();
            return;
        }
}
void Player::TakeDamage()
{
    if (IsFlagSet(_playerFlags, PlayerFlags::IsInvincible))
        return;
    // If big, then power down
    SetFlag(_playerFlags, PlayerFlags::IsSuper, false);
    if (IsFlagSet(_playerFlags, PlayerFlags::IsBig))
    {
        powerDownSfx->Play();
        // Powerup(false);
        PowerChangeStart(false);
        SetFlag(_playerFlags, PlayerFlags::IsInvincible, true);
        return;
    }
    // If small, die
    Die();
}
void Player::DeathBoxOverlap(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto player = (Player *)instance;
    if (player->_isDead || player->_isDying)
        return;
    player->Die();
}

void Player::WinBoxOverlap(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto player = (Player *)instance;
    auto winEvent = Event{player, nullptr, (int)EventTypes::PlayerWin};
    player->GetGame().PushEvent(winEvent);
    player->Win();
}
void Player::FlagOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto player = (Player *)instance;
    auto flag = static_cast<Flag *>(static_cast<gpBody*>(overlapBody)->funcArgs);
    if (!flag)
        return;
    flag->TakeDamage();
    player->SlideFunc();
    player->_rigidbodyComponent->GravityEnabled(true);
}

void Player::Die()
{
    auto bigEvent = Event{this, this, (int)EventTypes::PlayerDie};
    GetGame().PushEvent(bigEvent);
    dieBgm->Play(0);
    _isDying = true;
    _rigidbodyComponent->SetCollidesWithStaticBody(false);
    _rigidbodyComponent->Velocity().x = 0;
    _rigidbodyComponent->Velocity().y = 0;
    _currentDeadTime = 0;
}

void Player::Win()
{
    if (!_isWinning)
    {
        _isWinning = true;
        _rigidbodyComponent->Velocity().x = 0;
        _rigidbodyComponent->Velocity().y = 0;
        _rigidbodyComponent->GravityEnabled(false);
        SetFlag(_playerFlags, PlayerFlags::IsClimbing, true);
        whistleSfx->Play();
        _animationComponent->Mirror = false;
        _shouldClimbAnim = true;
        _animationComponent->Update();
        _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Static, &EndLevelStaticOverlapFunc);
        _boxColliderComponent->AddOverlapFunction((int)BodyTypes::Flag, &Player::FlagOverlapFunc);
        return;
    }
}

void Player::BrickOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    if (player->_isDead || player->_isDying)
        return;
    if (player->_rigidbodyComponent->Velocity().y > 0 && player->_rigidbodyComponent->Acceleration().y > 0)
        return;
    ItemBrick *itemBox = (ItemBrick *)((gpBody* )overlapBody)->funcArgs;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapUp)
    {
        itemBox->TakeDamage();
    }
}

void Player::ItemBoxOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    if (player->_isDead || player->_isDying)
        return;
    // If we are not traveling upwards
    if (player->_rigidbodyComponent->Velocity().y > 0 && player->_rigidbodyComponent->Acceleration().y > 0)
        return;
    ItemBox *itemBox = (ItemBox *)((gpBody*) overlapBody)->funcArgs;
    if (overlap->overlapDirection == gpOverlapDirections::gpOverlapUp)
    {
        itemBox->TakeDamage();
    }
}

void Player::MushroomOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    if (player->_isDead || player->_isDying)
        return;
    Mushroom *mushroom = (Mushroom *)((gpBody*)overlapBody)->funcArgs;
    if (!mushroom->IsEnabled())
        return;
    if (!player->IsFlagSet(player->_playerFlags, PlayerFlags::IsBig))
        player->PowerChangeStart(true);
    mushroom->TakeDamage();
}
void Player::FireflowerOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    if (player->_isDead || player->_isDying)
        return;
    Fireflower *flower = (Fireflower *)((gpBody*)overlapBody)->funcArgs;
    if (!flower->IsEnabled())
        return;
    if (!player->IsFlagSet(player->_playerFlags, PlayerFlags::IsBig))
    {
        player->PowerChangeStart(true);
    }
    else
    {
        player->SetFlag(player->_playerFlags, Player::PlayerFlags::IsSuper, true);
        player->SuperPowerupTick();
    }
    flower->TakeDamage();
}

void Player::CoinOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    if (player->_isDead || player->_isDying)
        return;
    Coin *coin = (Coin *)((gpBody *)overlapBody)->funcArgs;
    if (!coin->IsEnabled())
        return;
    ++player->_coinsCollected;
    coin->TakeDamage();
    auto coinEvent = Event{player, (void *)&player->_coinsCollected, (int)EventTypes::CoinCollected};
    player->GetGame().PushEvent(coinEvent);
}

void Player::EndLevelStaticOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    Player *player = static_cast<Player *>(instance);
    switch (overlap->overlapDirection)
    {
    case gpOverlapRight:
        player->_animationComponent->Visible(false);
        player->SetFlag(player->_playerFlags, PlayerFlags::CanExitLevel, true);
        break;
    }
}

void Player::PowerChangeStart(bool isGettingBig)
{
    if (!_isTurningBig)
    {
        SetFlag(_playerFlags, PlayerFlags::IsBig, isGettingBig);
        _currentBigIterations = 0;
        _currentBigIterationTime = 0;
        auto bigEvent = Event{this, this, (int)EventTypes::PlayerBig};
        GetGame().PushEvent(bigEvent);
        _isTurningBig = true;
    }
}

void Player::PowerupTick()
{
    // End
    if (_currentBigIterations > _bigIterations)
    {
        _isTurningBig = false;
        auto bigEvent = Event{this, nullptr, (int)EventTypes::PlayerBig};
        GetGame().PushEvent(bigEvent);
        if (IsFlagSet(_playerFlags, PlayerFlags::IsBig))
        {
            // Move marios current location up by 16 pixes cause he is getting big
            _location.y -= 16;
            // Adjust rigidbody location upwards and increase height
            _rigidbodyComponent->BoundingBox().y -= 16;
            _rigidbodyComponent->BoundingBox().h += 16;
            _boxColliderComponent->BoundingBox().y -= 16;
            _boxColliderComponent->BoundingBox().h += 16;
            // Adjust animation offset
            _animationComponent->Offset(Point{0, -6});
        }
        else
        {
            _location.y += 16;
            _rigidbodyComponent->BoundingBox().y += 16;
            _rigidbodyComponent->BoundingBox().h -= 16;
            _boxColliderComponent->BoundingBox().y += 16;
            _boxColliderComponent->BoundingBox().h -= 16;
            _animationComponent->Offset(Point{0, -20});
        }
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
            // Handle where our offset should be, as our location is different per.
            auto wasPreviouslyBig = !IsFlagSet(_playerFlags, PlayerFlags::IsBig);
            auto isBig = endsWith(newName, "b") || endsWith(newName, "f") || endsWith(newName, "ft");
            if (isBig)
            {
                auto removeChars = endsWith(newName, "ft") ? 2 : 1;
                newName.erase(newName.size() - removeChars);
                if (wasPreviouslyBig)
                {
                    _animationComponent->Offset(Point{0, -6});
                }
            }
            else
            {
                newName = newName + "b";
                if (wasPreviouslyBig)
                {
                    _animationComponent->Offset(Point{0, -8});
                }
            }
            _animationComponent->ChangeAnimation(newName);
        }
    }
}
void Player::SuperPowerupTick()
{
    if (!_isTurningBig)
    {
        _isTurningBig = true;
        _currentBigIterations = 0;
        _currentBigIterationTime = 0;
        auto bigEvent = Event{this, this, (int)EventTypes::PlayerBig};
        GetGame().PushEvent(bigEvent);
    }

    // End
    if (_currentBigIterations > _bigIterations)
    {
        _isTurningBig = false;
        auto bigEvent = Event{this, nullptr, (int)EventTypes::PlayerBig};
        GetGame().PushEvent(bigEvent);
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
                newName = newName + "f";
            }
            else
            {
                newName.erase(newName.size() - 1);
                newName = newName + "b";
            }
            _animationComponent->ChangeAnimation(newName);
        }
    }
}

void Player::SlideFunc()
{
    _rigidbodyComponent->GravityEnabled(true);
    winBgm->Play(0, 0.5);
    _isWinning = false;
    _isWinWalking = true;
    _currentDeadTime = 0;
}

void Player::WinWalking()
{
    if (IsFlagSet(_playerFlags, PlayerFlags::IsClimbing))
    {
        if (_rigidbodyComponent->IsOnGround())
            SetFlag(_playerFlags, PlayerFlags::IsClimbing, false);
    }
    _rigidbodyComponent->Velocity().x = IsFlagSet(_playerFlags, PlayerFlags::IsClimbing) ? 0 : 45;
    _shouldClimbAnim = IsFlagSet(_playerFlags, PlayerFlags::IsClimbing) ? true : false;
}