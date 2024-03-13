#include <GoonWorld/gameobjects/Mushroom.hpp>
#include <SupergoonSound/include/sound.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>

using namespace GoonWorld;

static gsSfx *mushroomSound;

Mushroom::Mushroom(SDL_Rect *rect)
    : GameObject(rect), _startedMoving(false)
{
    // _debugDrawComponent = new DebugDrawComponent(Point{rect->w, rect->h});
    _rigidbodyComponent = new RigidbodyComponent(rect);
    _rigidbodyComponent->SetBodyType(BodyTypes::Mushroom);
    auto staticOverlapArgs = bodyOverlapArgs{(int)BodyTypes::Mushroom, (int)BodyTypes::Static, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                                             {
                                                 Mushroom *mushroom = static_cast<Mushroom *>(args);
                                                 mushroom->MushroomStaticBodyOverlap(overlap);
                                             }};

    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, staticOverlapArgs);
    _animationComponent = new AnimationComponent("mushroom");
    _animationComponent->SizeMultiplier = 2;
    _animationComponent->AddTransition("idle", "walk", true, &_startedMoving);
    mushroomSound = (gsSfx *)Content::LoadContent(ContentTypes::Sfx, "mushroom");
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
    gsPlaySfxOneShot(mushroomSound, 1.0f);
    Enabled(false);
}
void Mushroom::Push(bool right)
{
    _startedMoving = true;
    _movingRight = right;
    _rigidbodyComponent->Velocity().x = _movingRight ? _moveSpeed : -_moveSpeed;
}

void Mushroom::MushroomStaticBodyOverlap(gpOverlap *overlap)
{
    switch (overlap->overlapDirection)
    {
    case gpOverlapDown:
    case gpOverlapUp:
        break;
    case gpOverlapLeft:
        _movingRight = true;
        break;
    case gpOverlapRight:
        _movingRight = false;
        break;
    }
}