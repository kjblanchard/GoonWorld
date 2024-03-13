#include <GoonWorld/gameobjects/DeathBox.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonEngine/rectangle.h>
using namespace GoonWorld;

DeathBox::DeathBox(TiledMap::TiledObject &object)
{
        auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
        _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
        _rigidbodyComponent->GravityEnabled(false);
        _rigidbodyComponent->SetBodyType(BodyTypes::DeathBox);
        AddComponent(_rigidbodyComponent);
}