#include <GoonWorld/gameobjects/Fireflower.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>

using namespace GoonWorld;

Fireflower::Fireflower(geRectangle *rect)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Fireflower);
    _animationComponent = new AnimationComponent("fireflower");
    AddComponent({_rigidbodyComponent, _animationComponent});
}

void Fireflower::TakeDamage()
{
    if (!IsEnabled())
        return;
    Enabled(false);
}