#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/scene.h>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;

std::vector<RigidbodyComponent *> RigidbodyComponent::_currentRigidbodies;

RigidbodyComponent::RigidbodyComponent(geRectangle *rect)
    : Component((int)ComponentTypes::Rigidbody), _static(false)
{
    auto bb = gpBBNew(rect->x, rect->y, rect->w, rect->h);
    _body = gpBodyNew(bb);
    _body->gravityEnabled = 1;
    _body->updateFunc = &OnBodyUpdate;
    _currentRigidbodies.push_back(this);
}
RigidbodyComponent::~RigidbodyComponent()
{
    gpSceneRemoveBody(_bodyNum);
}

void RigidbodyComponent::AddOverlapFunction(int overlapType, OverlapFunc func)
{
    auto args = bodyOverlapArgs{_body->bodyType, overlapType, func};
    gpBodyAddOverlapBeginFunc(_body, args);
}

bool RigidbodyComponent::IsOnGround()
{
    if (Game::GetTicks() == _isOnGroundCached)
        return _isOnGround;
    _isOnGroundCached = Game::GetTicks();
    return _isOnGround = gpBodyIsOnGround(_body);
}

void RigidbodyComponent::OnComponentAdd(GameObject &parent)
{
    _body->funcArgs = (void *)&parent;
    Component::OnComponentAdd(parent);
    if (_static)
    {
        _body->staticBody = true;
        _bodyNum = gpSceneAddStaticBody(_body);
    }
    else
    {
        _bodyNum = gpSceneAddBody(_body);
    }
}

void RigidbodyComponent::OnEnabled()
{
    _body->gravityEnabled = _isGravityEnabled;
    Component::OnEnabled();
}

void RigidbodyComponent::OnDisabled()
{
    _body->gravityEnabled = false;
    Component::OnDisabled();
}

void RigidbodyComponent::OnBodyUpdate(void *args, gpBody *body)
{
    auto gameobject = static_cast<GameObject *>(args);
    if (!gameobject)
        return;
    gameobject->Location().x = body->boundingBox.x;
    gameobject->Location().y = body->boundingBox.y;
}