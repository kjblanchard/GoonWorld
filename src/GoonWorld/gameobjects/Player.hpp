#pragma once
#include <GoonWorld/base/GameObject.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class Player : public GameObject
    {
    public:
        Player(TiledMap::TiledObject &object);
        void Update() override;

        ~Player();

    private:
        DebugDrawComponent *_debugDrawComponent;
        PlayerInputComponent *_playerInputComponent;
    };
}