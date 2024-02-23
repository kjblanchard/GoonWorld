#pragma once
#include <memory>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonEngine/point.h>
namespace GoonWorld
{
    class Component;
    class GameObject : IUpdate, IStart
    {
    public:
        GameObject();
        GameObject(TiledMap::TiledObject);
        virtual ~GameObject();
        virtual void Start() const override;
        virtual void Update() const override;
        inline unsigned int Id() { return _id; }
        inline Point Location() { return _location; }
        void AddComponent(Component *component);
        template <typename... Components>
        inline void AddComponent(Components *...components)
        {
            AddComponent(components...);
        }
        Component *GetComponent(unsigned int componentType);
        template <typename T>
        T *GetComponentOfType(unsigned int componentType);

    protected:
        static TimeSpan _deltaTime;
        unsigned int _id;
        Point _location;
        std::vector<std::shared_ptr<Component>> _components;

    private:
        static unsigned int _numGameObjects;
    };

    template <typename T>
    T *GameObject::GetComponentOfType(unsigned int componentType)
    {
        auto thing = GetComponent(componentType);
        return thing ? static_cast<T>(thing) : nullptr;
    }

}