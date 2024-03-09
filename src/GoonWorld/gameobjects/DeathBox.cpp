#include <GoonWorld/gameobjects/DeathBox.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
using namespace GoonWorld;

DeathBox::DeathBox(TiledMap::TiledObject &object)
{
        auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
        _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
        _rigidbodyComponent->GravityEnabled(false);
        _rigidbodyComponent->SetBodyType(BodyTypes::DeathBox);
        AddComponent(_rigidbodyComponent);
}