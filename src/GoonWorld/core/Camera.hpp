#pragma once

#include <SDL2/SDL_types.h>
#include <GoonEngine/point.h>

namespace GoonWorld
{
    class GameObject;
    class Camera
    {
    public:
        Camera(SDL_Rect bounds);
        inline SDL_Rect *Bounds() { return &_cameraBounds; }
        inline void SetLevelSize(Point size) { levelSize = size; }
        inline void Restart()
        {
            _cameraBounds.x = 0;
            _cameraBounds.y = 0;
        }
        inline void SetFollowTarget(GameObject *target) { _followTarget = target; }
        void Update();

    private:
        SDL_Rect _cameraBounds;
        Point levelSize;
        GameObject *_followTarget;
    };

}