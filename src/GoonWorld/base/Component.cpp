#include <GoonWorld/base/Component.hpp>
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
void Component::Update() const
{
}
Component::~Component()
{
    GameObjectComponentTypeMap[_parent->Id()].erase(_componentType);
}
void Component::OnComponentAdd(GameObject &parent)
{
    _parent = &parent;
    GameObjectComponentTypeMap[_parent->Id()][_componentType] = this;
}