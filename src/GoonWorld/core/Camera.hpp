#pragma once

#include <SDL2/SDL_types.h>

namespace GoonWorld
{
    class GameObject;
    class Camera
    {
    public:
        Camera(SDL_Rect bounds);
        inline SDL_Rect *Bounds() { return &_cameraBounds; }
        inline void SetFollowTarget(GameObject* target) { _followTarget = target;}
        void Update();

    private:
        SDL_Rect _cameraBounds;
        GameObject* _followTarget;

    };

}