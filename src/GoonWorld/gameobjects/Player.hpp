#pragma once
#include <GoonWorld/base/GameObject.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class Player : public GameObject
    {
    public:
        Player(TiledMap::TiledObject &object);
        ~Player();

    private:
        DebugDrawComponent *_debugDrawComponent;
    };
}