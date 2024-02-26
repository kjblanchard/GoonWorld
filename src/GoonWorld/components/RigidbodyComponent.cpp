#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/scene.h>
#include <SDL2/SDL_rect.h>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;

std::vector<RigidbodyComponent *> RigidbodyComponent::_currentRigidbodies;

RigidbodyComponent::RigidbodyComponent(SDL_Rect *rect)
    : Component((int)ComponentTypes::Rigidbody)
{
    auto bb = gpBBNew(rect->x, rect->y, rect->w, rect->h);
    _body = gpBodyNew(bb);
    _bodyNum = gpSceneAddBody(_body);
    _body->gravityEnabled = 1;
    _currentRigidbodies.push_back(this);
    // Location = Point{rect->x, rect->y};
    // Size = Point{rect->w, rect->h};
}
void RigidbodyComponent::PhysicsUpdate()
{
    for (auto rigidbody : _currentRigidbodies)
    {
        if (!rigidbody->Parent())
            continue;

        rigidbody->Parent()->Location().x = rigidbody->_body->boundingBox.x;
        rigidbody->Parent()->Location().y = rigidbody->_body->boundingBox.y;
    }
}