#pragma once
#include <GoonWorld/base/GameObject.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class PlayerInputComponent;
    class RigidbodyComponent;
    class Player : public GameObject
    {
    public:
        Player(TiledMap::TiledObject &object);
        void Update() override;

        ~Player();

    private:
        DebugDrawComponent *_debugDrawComponent;
        PlayerInputComponent *_playerInputComponent;
        RigidbodyComponent *_rigidbodyComponent;
    };
}