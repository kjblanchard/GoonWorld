#pragma once
#include <GoonWorld/base/GameObject.hpp>
namespace GoonWorld
{
    class RigidbodyComponent;
    class WinBox : public GameObject
    {
    public:
        WinBox(TiledMap::TiledObject &object);

    private:
        RigidbodyComponent* _rigidbodyComponent;
    };
}