#pragma once

#include <GoonWorld/base/GameObject.hpp>

namespace GoonWorld
{
    class DebugDrawComponent;
    class Player : GameObject
    {
    public:
        Player();
        ~Player();

    private:
        DebugDrawComponent *_debugDrawComponent;
    };
}