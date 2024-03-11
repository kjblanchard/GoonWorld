#pragma once
#include <memory>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonEngine/point.h>

typedef struct SDL_Rect SDL_Rect;
namespace GoonWorld
{
    class Component;
    class GameObject : public IUpdate, public IStart, public IEnable
    {
    public:
        GameObject();
        GameObject(SDL_Rect* rect);
        GameObject(TiledMap::TiledObject);
        virtual ~GameObject();
        virtual void Start() const override;
        virtual void Update() override;
        inline unsigned int Id() { return _id; }
        inline Point &Location() { return _location; }
        inline bool IsEnabled() const override {return _enabled;}
        virtual void Enabled(bool isEnabled) override;
        virtual void OnEnabled() override;
        virtual void OnDisabled() override;
        void AddComponent(Component *component);
        void AddComponent(std::initializer_list<Component *> componentList);
        Component *GetComponent(unsigned int componentType);
        template <typename T>
        T *GetComponentOfType(unsigned int componentType);
        static TimeSpan DeltaTime;
        // TODO make this private
        static std::vector<std::shared_ptr<GameObject>> _gameobjects;

    protected:
        unsigned int _id;
        Point _location;
        std::vector<std::shared_ptr<Component>> _components;

    private:
        static unsigned int _numGameObjects;
        bool _enabled;
    };

    template <typename T>
    T *GameObject::GetComponentOfType(unsigned int componentType)
    {
        auto thing = GetComponent(componentType);
        return thing ? static_cast<T>(thing) : nullptr;
    }
}