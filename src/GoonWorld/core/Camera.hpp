#pragma once

#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>

namespace GoonWorld
{
    class GameObject;
    class Camera
    {
    public:
        Camera(geRectangle bounds);
        inline geRectangle *Bounds() { return &_cameraBounds; }
        inline void SetLevelSize(Point size) { levelSize = size; }
        bool IsPointOnCamera(Point point);
        inline void Restart()
        {
            _cameraBounds.x = 0;
            _cameraBounds.y = 0;
        }
        inline void SetFollowTarget(GameObject *target) { _followTarget = target; }
        void Update();

    private:
        geRectangle _cameraBounds;
        Point levelSize;
        GameObject *_followTarget;
    };

}