#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;
Camera::Camera(SDL_Rect cameraBounds) : _cameraBounds(cameraBounds), _followTarget(nullptr), levelSize(Point{0, 0})
{
}

void Camera::Update()
{
    if (!_followTarget)
        return;
    int halfwayPointX = _cameraBounds.w / 2;
    int maxCameraX = levelSize.x - _cameraBounds.w;
    if (_followTarget->Location().x > halfwayPointX)
        // Cap the camera at the end of the level, else set it halfway
        _cameraBounds.x = std::min(maxCameraX, _followTarget->Location().x - halfwayPointX);
}