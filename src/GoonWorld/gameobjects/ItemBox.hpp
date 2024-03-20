#pragma once
typedef struct gpOverlap gpOverlap;
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/interfaces/ITakeDamage.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class RigidbodyComponent;
    class AnimationComponent;
    class Sfx;
    class ItemBox : public GameObject, ITakeDamage
    {
    public:
        ItemBox(TiledMap::TiledObject &object);
        void TakeDamage() override;

    private:
        DebugDrawComponent *_debugDrawComponent;
        RigidbodyComponent *_rigidbodyComponent;
        AnimationComponent *_animationComponent = nullptr;
        bool _isOpened = false;
        int _contents = 0;
        Sfx *bumpSfx = nullptr;
        Sfx *powerupSpawnSfx = nullptr;

        enum class ItemBrickContents
        {
            Empty,
            Mushroom,
            Fireflower,
        };
    };

}