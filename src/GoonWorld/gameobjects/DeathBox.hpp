#pragma once
#include <GoonWorld/base/GameObject.hpp>
typedef struct gpOverlap gpOverlap;
namespace GoonWorld
{
    class RigidbodyComponent;
    class DeathBox : public GameObject
    {
    public:
        DeathBox(TiledMap::TiledObject &object);

    private:
        RigidbodyComponent* _rigidbodyComponent;
    };
}