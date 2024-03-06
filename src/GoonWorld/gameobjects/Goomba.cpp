#include <GoonWorld/gameobjects/Goomba.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
using namespace GoonWorld;

Goomba::Goomba(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _rigidbodyComponent->SetBodyType(2);
    _animationComponent = new AnimationComponent("goomba");
    _animationComponent->SizeMultiplier = 2;
    AddComponent({_debugDrawComponent, _rigidbodyComponent, _animationComponent});
}
void Goomba::GoombaOverlap()
{
    auto id = _id;
    printf("Goomba id, %d\n", _id);
}
void Goomba::Update()
{
    _animationComponent->Mirror = _rigidbodyComponent->Velocity().x < 0;
    if (_rigidbodyComponent->Velocity().x == 0)
    {
        _rigidbodyComponent->Velocity().x = -25;
    }
    _rigidbodyComponent->Acceleration().x -= 150 * DeltaTime.GetTotalSeconds();
    GameObject::Update();
}
Goomba::~Goomba()
{
}