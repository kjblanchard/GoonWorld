#include <GoonWorld/gameobjects/Fireball.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonEngine/debug.h>
// #include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Sfx.hpp>

using namespace GoonWorld;

const char *fireballSound = "fireball";
const char *fireballDeadSound = "death";
static Sfx *fireballSfx;
static Sfx *fireballDeadSfx;

Fireball::Fireball(geRectangle *rect, std::queue<Fireball *> *home)
    : GameObject(rect), _fireballHome(home)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Fireball);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Static, &StaticBodyOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::ItemBox, &StaticBodyOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::ItemBrick, &StaticBodyOverlapFunc);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Goomba, &GoombaOverlapFunc);
    _animationComponent = new AnimationComponent("fireball");
    _animationComponent->SizeMultiplier = 1;
    fireballSfx = Sfx::SfxFactory(fireballSound);
    fireballDeadSfx = Sfx::SfxFactory(fireballDeadSound);
    AddComponent({_rigidbodyComponent, _animationComponent});
    Enabled(false);
}
void Fireball::Update()
{
    // Check to see if we are off the camera screen
    auto directionOffScreen = _movingRight ? -16 : 16;
    auto shouldCull = !Game::Instance()->GetCamera()->IsPointOnCamera(Point{_location.x + directionOffScreen, _location.y});
    if (shouldCull)
    {
        End();
    }
    _rigidbodyComponent->Velocity().x = _movingRight ? _moveSpeed : -_moveSpeed;
    if (_hitGround)
    {
        _hitGround = false;
        _rigidbodyComponent->Velocity().y = _jumpVelocity;
    }
    // printf("Fireball vel is %f:%f\n", _rigidbodyComponent->Velocity().x, _rigidbodyComponent->Velocity().y);
    GameObject::Update();
}
void Fireball::TakeDamage()
{
    if (!IsEnabled())
        return;
    Enabled(false);
}
void Fireball::Push(Point spawnLocation, bool right)
{
    Enabled(true);
    _rigidbodyComponent->SetLocation(spawnLocation);
    _location = spawnLocation;
    _rigidbodyComponent->Velocity().x = right ? _moveSpeed : -_moveSpeed;
    // LogWarn("Fireball vel: %f:%f  Acc: %f:%f", _rigidbodyComponent->Velocity().x, _rigidbodyComponent->Velocity().y,
    //         _rigidbodyComponent->Acceleration().x, _rigidbodyComponent->Acceleration().y);
    _movingRight = right;
    fireballSfx->Play();
}
void Fireball::GoombaOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto fireball = (Fireball *)instance;
    Goomba *goomba = (Goomba *)((gpBody *)overlapBody)->funcArgs;
    if (goomba->IsDead())
        return;
    goomba->TakeDamage();
    fireball->End();
    return;
}

void Fireball::StaticBodyOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto fireball = static_cast<Fireball *>(instance);
    switch (overlap->overlapDirection)
    {
    case gpOverlapDown:
        fireball->_hitGround = true;
        break;
    case gpOverlapUp:
        fireball->_hitGround = true;
        break;
    case gpOverlapLeft:
    case gpOverlapRight:
        fireballDeadSfx->Play();
        fireball->End();
        break;
    }
}
void Fireball::End()
{
    _rigidbodyComponent->Velocity().x = 0;
    _rigidbodyComponent->Velocity().y = 0;
    _rigidbodyComponent->Acceleration().x = 0;
    _rigidbodyComponent->Acceleration().y = 0;
    Enabled(false);
    _fireballHome->push(this);
}