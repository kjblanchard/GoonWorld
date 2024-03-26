#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Sfx.hpp>
#include <GoonWorld/tween/Tween.hpp>

using namespace GoonWorld;

const char *mushroomSound = "mushroom";

Mushroom::Mushroom(geRectangle *rect)
    : GameObject(rect), _startedMoving(false)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Mushroom);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Static, &StaticBodyOverlapFunc);
    _animationComponent = new AnimationComponent("mushroom", Point{0, -1});
    _animationComponent->AddTransition("idle", "walk", true, &_startedMoving);
    mushroomSoundSfx = Sfx::SfxFactory(mushroomSound);
    AddComponent({_rigidbodyComponent, _animationComponent});
    Enabled(false);
}

void Mushroom::Update()
{
    if (_startedMoving)
        _rigidbodyComponent->Velocity().x = _movingRight ? _moveSpeed : -_moveSpeed;
    GameObject::Update();
}
void Mushroom::TakeDamage()
{
    if (!IsEnabled())
        return;
    mushroomSoundSfx->Play();
    Enabled(false);
}
void Mushroom::Push(bool right)
{
    _movingRight = right;
    Enabled(true);
    _rigidbodyComponent->GravityEnabled(false);
    auto tween = new Tween<int>(_location.y, _location.y - 16, 0.5, Easings::Linear);
    tween->SetCallback(TweenEndCallback);
    tween->SetCallbackArgs(this);
    GetGame().AddTween(tween);
}
void Mushroom::TweenEndCallback(void* args)
{
    auto mushroom = static_cast<Mushroom*>(args);
    mushroom->_rigidbodyComponent->BoundingBox().y = mushroom->_location.y;
    mushroom->_rigidbodyComponent->GravityEnabled(true);
    mushroom->_startedMoving = true;
    mushroom->_rigidbodyComponent->Velocity().x = mushroom->_movingRight ? mushroom->_moveSpeed : -mushroom->_moveSpeed;
}

void Mushroom::StaticBodyOverlapFunc(void *instance, void *body, void *overlapBody, gpOverlap *overlap)
{
    auto mushroom = static_cast<Mushroom *>(instance);
    switch (overlap->overlapDirection)
    {
    case gpOverlapDown:
    case gpOverlapUp:
        break;
    case gpOverlapLeft:
        mushroom->_movingRight = true;
        break;
    case gpOverlapRight:
        mushroom->_movingRight = false;
        break;
    }
}