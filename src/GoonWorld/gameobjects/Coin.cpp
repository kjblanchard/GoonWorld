#include <GoonWorld/gameobjects/Coin.hpp>
#include <GoonWorld/core/Sound.hpp>
// #include <GoonEngine/Sound.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>

using namespace GoonWorld;

// static gsSfx *coinsound;
const char* coinSound = "coin";

Coin::Coin(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
    // _debugDrawComponent = new DebugDrawComponent(Point{rect->w, rect->h});
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Coin);
    _rigidbodyComponent->GravityEnabled(false);

    _animationComponent = new AnimationComponent("coin");
    _animationComponent->SizeMultiplier = 1;
    // coinsound = (gsSfx *)Content::LoadContent(ContentTypes::Sfx, "coin");
    GetGameSound().LoadSfx(coinSound);
    AddComponent({_rigidbodyComponent, _animationComponent, _debugDrawComponent});
    _debugDrawComponent->Enabled(false);
}

// void Coin::Update()
// {
//     GameObject::Update();
// }
void Coin::TakeDamage()
{
    if (!IsEnabled())
        return;
        GetGameSound().PlaySfx(coinSound, 0.25);
    // gsPlaySfxOneShot(coinsound, 0.25f);
    Enabled(false);
}