#include <GoonPhysics/overlap.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonEngine/Sound.h>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
using namespace GoonWorld;

static Sfx *dieSound = nullptr;

Goomba::Goomba(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _movingRight = false;
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(2);
    _animationComponent = new AnimationComponent("goomba");
    _animationComponent->SizeMultiplier = 2;
    auto staticOverlapArgs = bodyOverlapArgs{(int)BodyTypes::Goomba, (int)BodyTypes::Static, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                                             {
                                                 Goomba *goombaInstance = static_cast<Goomba *>(args);
                                                 goombaInstance->GoombaStaticBodyOverlap(overlap);
                                             }};
    auto marioOverlapArgs = bodyOverlapArgs{(int)BodyTypes::Goomba, (int)BodyTypes::Player, [](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
                                            {
                                                Goomba *goombaInstance = static_cast<Goomba *>(args);
                                                goombaInstance->GoombaMarioOverlap(overlap);
                                            }};
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, staticOverlapArgs);
    // gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, marioOverlapArgs);
    if (!dieSound)
        dieSound = (Sfx *)Content::LoadContent(ContentTypes::Sfx, "death");
    // AddComponent({_debugDrawComponent, _rigidbodyComponent, _animationComponent});
    AddComponent({_rigidbodyComponent, _animationComponent});

    _animationComponent->AddTransition("walk", "dead", true, &_isDead);
}
void Goomba::TakeDamage()
{
    if (_isDead)
        return;
    PlaySfxOneShot(dieSound, 1.0f);
    _isDead = true;
}
void Goomba::GoombaMarioOverlap(gpOverlap *overlap)
{
    Player *player = (Player *)overlap->overlapBody->funcArgs;
    if (player->CanDamage())
        return;
    switch (overlap->overlapDirection)
    {
    case gpOverlapDirections::gpOverlapUp:
        if (_isDead)
            return;
        // LogInfo("I should die");
        PlaySfxOneShot(dieSound, 1.0f);
        _isDead = true;
        break;
    default:
        break;
    }
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
            _animationComponent->Enabled(false);
            return;
        }
    }
    auto speed = _movingRight ? 40 : -40;
    _rigidbodyComponent->Velocity().x = _isDead ? 0 : speed;
    GameObject::Update();
}
void Goomba::GoombaStaticBodyOverlap(gpOverlap *overlap)
{
    switch (overlap->overlapDirection)
    {
    case gpOverlapDown:
    case gpOverlapUp:
        break;
    case gpOverlapLeft:
        _animationComponent->Mirror = false;
        _movingRight = true;
        break;
    case gpOverlapRight:
        _animationComponent->Mirror = true;
        _movingRight = false;
        break;
    }
}
Goomba::~Goomba()
{
}