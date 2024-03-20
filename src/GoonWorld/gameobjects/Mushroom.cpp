#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonWorld/content/Sfx.hpp>

using namespace GoonWorld;

const char *mushroomSound = "mushroom";

Mushroom::Mushroom(geRectangle *rect)
    : GameObject(rect), _startedMoving(false)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Mushroom);
    _rigidbodyComponent->AddOverlapFunction((int)BodyTypes::Static, &StaticBodyOverlapFunc);
    _animationComponent = new AnimationComponent("mushroom");
    _animationComponent->SizeMultiplier = 1;
    _animationComponent->AddTransition("idle", "walk", true, &_startedMoving);
    mushroomSoundSfx = Sfx::SfxFactory(mushroomSound);
    AddComponent({_rigidbodyComponent, _animationComponent});
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
    _startedMoving = true;
    _movingRight = right;
    _rigidbodyComponent->Velocity().x = _movingRight ? _moveSpeed : -_moveSpeed;
}

void Mushroom::StaticBodyOverlapFunc(void *instance, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
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