#include <vector>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Level.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/scene.h>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/color.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/components/BoxColliderComponent.hpp>
using namespace GoonWorld;

std::vector<RigidbodyComponent *> RigidbodyComponent::_currentRigidbodies;

RigidbodyComponent::RigidbodyComponent(geRectangle *rect, Point offset)
    : Component((int)ComponentTypes::Rigidbody), _static(false), _offset(offset)
{
    auto bb = gpBBNew(rect->x, rect->y, rect->w, rect->h);
    bb.x += offset.x;
    bb.y += offset.y;
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
    // if (Game::Instance()->GetTicks() == _isOnGroundCached)
    //     return _isOnGround;
    // _isOnGroundCached = Game::GetTicks();
    // return _isOnGround = gpBodyIsOnGround(_body);
    return gpBodyIsOnGround(_body);
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
    Enabled(true);
    GetGame().GetCurrentLevel().AddDrawObject(this);
}

void RigidbodyComponent::OnEnabled()
{
    _body->gravityEnabled = _isGravityEnabled;
    SetOverlapsEnabled(true);
    Component::OnEnabled();
}

void RigidbodyComponent::OnDisabled()
{
    _body->gravityEnabled = false;
    SetOverlapsEnabled(false);
    Component::OnDisabled();
}

void RigidbodyComponent::OnBodyUpdate(void *args, gpBody *body)
{
    auto gameobject = static_cast<GameObject *>(args);
    if (!gameobject)
        return;
    // TODO this is probably slow.
    auto comp = gameobject->GetComponent((int)ComponentTypes::Rigidbody);
    auto rb = dynamic_cast<RigidbodyComponent *>(comp);
    if (!rb)
        return;
    // Update the parents location
    // gameobject->Location().x = body->boundingBox.x + rb->_offset.x;
    // gameobject->Location().y = body->boundingBox.y + rb->_offset.y;
    gameobject->Location().x = body->boundingBox.x - rb->_offset.x;
    gameobject->Location().y = body->boundingBox.y - rb->_offset.y;
    // Update all the attached box colliders locations
    for (auto box : rb->_boxColliders)
    {
        // box->SetLocation(Point{(int)body->boundingBox.x + box->Offset().x, (int)body->boundingBox.y + box->Offset().y});
        box->SetLocation(Point{(int)gameobject->Location().x + box->Offset().x, (int)gameobject->Location().y + box->Offset().y});
    }
}

void RigidbodyComponent::Draw()
{
    if (!_enabled || !_debugDraw)
        return;
    auto drawColor = geColor{255, 0, 0, 255};
    geRectangle dstRect{
        // (int)_body->boundingBox.x,
        // (int)_body->boundingBox.y,
        Parent()->Location().x + _offset.x,
        Parent()->Location().y + _offset.y,
        (int)_body->boundingBox.w,
        (int)_body->boundingBox.h};
    geDrawDebugRect(&dstRect, &drawColor);
}

void RigidbodyComponent::Visible(bool isVisible)
{
}

bool RigidbodyComponent::IsVisible()
{
    return true;
}