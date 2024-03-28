#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class RigidbodyComponent;
    class ItemBrick : public GameObject, ITakeDamage
    {
    public:
        ItemBrick(TiledMap::TiledObject &object);
        void Update() override;
        void TakeDamage() override;

    private:
        RigidbodyComponent *_rigidbodyComponent;
        int _contents;

        enum class ItemBrickContents
        {
            Empty,
            Mushroom,
        };
    };

}