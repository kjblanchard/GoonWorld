#include <GoonWorld/base/Component.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;
std::unordered_map<unsigned int, std::unordered_map<unsigned int, Component *>> Component::GameObjectComponentTypeMap;

Component::Component(int componentType)
    : _componentType(componentType)
{
}
void Component::Start() const
{
}
void Component::Update()
{
}
Component::~Component()
{
    GameObjectComponentTypeMap[_parent->Id()].erase(_componentType);
}
Game &Component::GetGame()
{
    return *Game::Instance();
}

void Component::OnComponentAdd(GameObject &parent)
{
    _parent = &parent;
    GameObjectComponentTypeMap[_parent->Id()][_componentType] = this;
    Enabled(true);
}
void Component::Enabled(bool isEnabled)
{
    if (isEnabled)
        OnEnabled();
    else
        OnDisabled();
}
void Component::OnEnabled()
{
    _enabled = true;
}
void Component::OnDisabled()
{
    _enabled = false;
}