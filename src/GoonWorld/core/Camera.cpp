#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;
Camera::Camera(geRectangle cameraBounds) : _cameraBounds(cameraBounds),  levelSize(Point{0, 0}), _followTarget(nullptr)
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
bool Camera::IsPointOnCamera(Point point)
{
    auto cameraViewX = _cameraBounds.x + _cameraBounds.w;

    if (point.x >= _cameraBounds.x && point.x <= cameraViewX)
        return true;
    return false;
}