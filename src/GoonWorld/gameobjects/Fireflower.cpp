#include <GoonWorld/gameobjects/Fireflower.hpp>
#include <GoonWorld/content/Sfx.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/tween/Tween.hpp>
#include <GoonWorld/core/Game.hpp>

using namespace GoonWorld;

extern const char *mushroomSound;
static Sfx *mushroomSfx;

Fireflower::Fireflower(geRectangle *rect)
    : GameObject(rect)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Fireflower);
    _animationComponent = new AnimationComponent("fireflower", Point{-1, 0});
    AddComponent({_rigidbodyComponent, _animationComponent});
    mushroomSfx = Sfx::SfxFactory(mushroomSound);
    Enabled(false);
}

void Fireflower::Push()
{
    Enabled(true);
    _rigidbodyComponent->GravityEnabled(false);
    auto tween = new Tween<int>(_location.y, _location.y - 16, 0.5, Easings::Linear);
    tween->SetCallback(TweenEndCallback);
    tween->SetCallbackArgs(this);
    GetGame().AddTween(tween);
}

void Fireflower::TakeDamage()
{
    if (!IsEnabled())
        return;
    mushroomSfx->Play();
    Enabled(false);
}

void Fireflower::TweenEndCallback(void* args)
{
    auto flower = static_cast<Fireflower*>(args);
    flower->_rigidbodyComponent->BoundingBox().y = flower->_location.y;
}