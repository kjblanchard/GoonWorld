#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/base/Component.hpp>
using namespace GoonWorld;

unsigned int GameObject::_numGameObjects = 0;
TimeSpan GameObject::DeltaTime = TimeSpan(0);

GameObject::GameObject()
    : _id(_numGameObjects++), _location(Point{0, 0})
{
    Game::Instance()->UpdateObjects.push_back(std::shared_ptr<IUpdate>(this));
    _enabled = true;
}
GameObject::GameObject(SDL_Rect *rect)
    : GameObject()
{
    _location = Point{rect->x, rect->y};
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
void GameObject::Update()
{
    // if (!IsEnabled())
    //     return;
    for (auto &component : _components)
    {
        // if (!component->IsEnabled())
        //     continue;
        component->Update();
    }
}
void GameObject::AddComponent(Component *component)
{
    component->OnComponentAdd(*this);
    _components.push_back(std::shared_ptr<Component>(component));
}
void GameObject::AddComponent(std::initializer_list<Component *> componentList)
{
    for (auto component : componentList)
    {
        AddComponent(component);
    }
}
Component *GameObject::GetComponent(unsigned int componentType)
{
    auto it = Component::GameObjectComponentTypeMap[_id].find(componentType);
    if (it == Component::GameObjectComponentTypeMap[_id].end())
        return nullptr;
    return it->second;
}
void GameObject::Enabled(bool isEnabled)
{
    if (_enabled == isEnabled)
        return;
    _enabled = isEnabled;
    if (_enabled)
        OnEnabled();
    else
        OnDisabled();
}
void GameObject::OnEnabled()
{
    for (auto component : _components)
    {
        component->Enabled(true);
    }
}
void GameObject::OnDisabled()
{
    for (auto component : _components)
    {
        component->Enabled(false);
    }
}
