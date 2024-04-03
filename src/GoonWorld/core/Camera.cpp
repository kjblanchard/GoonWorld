#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/Camera.h>
using namespace GoonWorld;
Camera::Camera(geRectangle cameraBounds) : _cameraBounds(cameraBounds), levelSize(Point{0, 0}), _followTarget(nullptr)
{
    _camera = geCameraNew();
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
    else
    {
        _cameraBounds.x = 0;
    }

    // Apply scale
    geCameraSetX(_camera, _cameraBounds.x * 2);
    geCameraUpdate(_camera);
}
bool Camera::IsPointOnCamera(Point point)
{
    auto cameraViewX = _cameraBounds.x + _cameraBounds.w;

    if (point.x >= _cameraBounds.x && point.x <= cameraViewX)
        return true;
    return false;
}