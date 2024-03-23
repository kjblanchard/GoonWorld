#include <GoonPhysics/overlap.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Sfx.hpp>
using namespace GoonWorld;

const char *deadSound = "death";
static Sfx *deadSfx;

Goomba::Goomba(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _movingRight = false;
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    bodyRect.w -= 2;
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect, Point{-1, 0});
    _rigidbodyComponent->SetBodyType(2);
    _animationComponent = new AnimationComponent("goomba");
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Static, &StaticBodyOverlapFunc);
    _rigidbodyComponent->SetDebug(true);
    deadSfx = Sfx::SfxFactory(deadSound);
    AddComponent({_rigidbodyComponent, _animationComponent, _debugDrawComponent});
    _debugDrawComponent->Enabled(false);
    _animationComponent->AddTransition("walk", "dead", true, &_isDead);
}
void Goomba::TakeDamage()
{
    if (_isDead)
        return;
    deadSfx->Play();
    _isDead = true;
    _rigidbodyComponent->GravityEnabled(false);
}

void Goomba::Update()
{
    if (_isDead)
    {
        if (_currentDeadTime < _deadTimer)
        {
            _currentDeadTime += DeltaTime.GetTotalSeconds();
            _rigidbodyComponent->Enabled(false);
        }
        else
        {
            Enabled(false);
            return;
        }
    }
    auto speed = _movingRight ? 40 : -40;
    _rigidbodyComponent->Velocity().x = _isDead ? 0 : speed;
    GameObject::Update();
}
void Goomba::StaticBodyOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
{
    auto goomba = static_cast<Goomba *>(instance);
    switch (overlap->overlapDirection)
    {
    case gpOverlapDown:
    case gpOverlapUp:
        break;
    case gpOverlapLeft:
        goomba->_animationComponent->Mirror = false;
        goomba->_movingRight = true;
        break;
    case gpOverlapRight:
        goomba->_animationComponent->Mirror = true;
        goomba->_movingRight = false;
        break;
    }
}
Goomba::~Goomba()
{
}