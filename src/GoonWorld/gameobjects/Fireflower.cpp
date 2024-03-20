#include <GoonWorld/gameobjects/Fireflower.hpp>
#include <GoonWorld/content/Sfx.hpp>
#include <GoonWorld/core/Sound.hpp>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>

using namespace GoonWorld;

extern const char *mushroomSound;
static Sfx *mushroomSfx;

Fireflower::Fireflower(geRectangle *rect)
{
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Fireflower);
    _animationComponent = new AnimationComponent("fireflower");
    AddComponent({_rigidbodyComponent, _animationComponent});
    mushroomSfx = Sfx::SfxFactory(mushroomSound);
    // GetGameSound().LoadSfx(mushroomSound);
}

void Fireflower::TakeDamage()
{
    if (!IsEnabled())
        return;
    mushroomSfx->Play();
    Enabled(false);
}