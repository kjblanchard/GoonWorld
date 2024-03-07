#include <GoonPhysics/overlap.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
using namespace GoonWorld;

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
    gpBodyAddOverlapBeginFunc(_rigidbodyComponent->_body, marioOverlapArgs);
    AddComponent({_debugDrawComponent, _rigidbodyComponent, _animationComponent});
}
void Goomba::GoombaMarioOverlap(gpOverlap *overlap)
{
    switch (overlap->overlapDirection)
    {
    case gpOverlapDirections::gpOverlapUp:
        LogInfo("I should die");
        break;
    default:
        break;
    }
}

void Goomba::Update()
{
    auto speed = _movingRight ? 40 : -40;
    _rigidbodyComponent->Velocity().x = speed;
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