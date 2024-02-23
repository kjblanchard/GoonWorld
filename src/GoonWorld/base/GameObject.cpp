#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/base/Component.hpp>
using namespace GoonWorld;

unsigned int GameObject::_numGameObjects = 0;
TimeSpan _deltaTime = TimeSpan(0);

GameObject::GameObject()
    : _id(_numGameObjects++), _location(Point{32, 32})
{
    Game::Instance()->UpdateObjects.push_back(std::shared_ptr<IUpdate>(this));
}
GameObject::GameObject(TiledMap::TiledObject)
    : GameObject()
{
}
GameObject::~GameObject()
{
    Component::GameObjectComponentTypeMap.erase(_id);
}
void GameObject::Start() const
{
}
void GameObject::Update() const
{
    for (auto component : _components)
    {
        component->Update();
    }
}
void GameObject::AddComponent(Component *component)
{
    component->OnComponentAdd(*this);
    _components.push_back(std::shared_ptr<Component>(component));
}
Component *GameObject::GetComponent(unsigned int componentType)
{
    auto it = Component::GameObjectComponentTypeMap[_id].find(componentType);
    if (it == Component::GameObjectComponentTypeMap[_id].end())
        return nullptr;
    return it->second;
}
