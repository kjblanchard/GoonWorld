#include <GoonWorld/gameobjects/WinBox.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonEngine/rectangle.h>
using namespace GoonWorld;

WinBox::WinBox(TiledMap::TiledObject &object)
{
        auto bodyRect = geRectangle{object.X, object.Y, object.Width, object.Height};
        _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
        _rigidbodyComponent->GravityEnabled(false);
        _rigidbodyComponent->SetBodyType(BodyTypes::WinBox);
        AddComponent(_rigidbodyComponent);
}