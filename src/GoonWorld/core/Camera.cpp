#include <GoonWorld/core/Camera.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;
Camera::Camera(SDL_Rect cameraBounds) : _cameraBounds(cameraBounds), _followTarget(nullptr)
{
}

void Camera::Update()
{
    if (!_followTarget)
        return;
    _cameraBounds.x = _followTarget->Location().x;
    // _cameraBounds.y = _followTarget->Location().y;
}