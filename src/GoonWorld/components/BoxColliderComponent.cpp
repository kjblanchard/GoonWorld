#include <GoonWorld/components/BoxColliderComponent.hpp>
#include <GoonPhysics/scene.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;

std::vector<BoxColliderComponent *> BoxColliderComponent::_currentBoxColliders;

BoxColliderComponent::BoxColliderComponent(geRectangle *rect, Point offset)
    : Component((int)ComponentTypes::BoxCollider), _offset(offset)
{
    auto bb = gpBBNew(rect->x, rect->y, rect->w, rect->h);
    _boxCollider = gpBoxColliderNew(bb);
    _currentBoxColliders.push_back(this);
}
BoxColliderComponent::~BoxColliderComponent()
{
    gpSceneRemoveBoxCollider(_boxNum);
}
void BoxColliderComponent::AddOverlapFunction(int overlapType, OverlapFunc func)
{
}
void BoxColliderComponent::OnEnabled()
{
    Component::OnEnabled();
}
void BoxColliderComponent::OnDisabled()
{
    Component::OnDisabled();
}
void BoxColliderComponent::OnComponentAdd(GameObject &parent)
{
    _boxCollider->funcArgs = (void *)&parent;
    Component::OnComponentAdd(parent);
    _boxNum = gpSceneAddBoxCollider(_boxCollider);
    GetGame().AddDrawObject(this);
    Enabled(true);
}

void BoxColliderComponent::Draw()
{
    if (!_enabled || !_debugDraw)
        return;
    auto drawColor = geColor{0, 255, 255, 255};
    auto loc = _parent->Location();
    geRectangle dstRect{
        (int)loc.x,
        (int)loc.y,
        (int)_boxCollider->boundingBox.w,
        (int)_boxCollider->boundingBox.h};
    geDrawDebugRect(&dstRect, &drawColor);
}

void BoxColliderComponent::Visible(bool isVisible)
{
}

bool BoxColliderComponent::IsVisible()
{
    return true;
}