#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    // class AnimationComponent;
    class Player;
    class ItemBrick : public GameObject, ITakeDamage
    {
    public:
        ItemBrick(TiledMap::TiledObject &object);
        void Update() override;
        void TakeDamage() override;

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        int _contents;

        enum class ItemBrickContents
        {
            Empty,
            Mushroom,
        };
    };

}