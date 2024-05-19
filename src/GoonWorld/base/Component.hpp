#pragma once
#include <map>
#include <unordered_map>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/ComponentTypes.hpp>

namespace GoonWorld
{
    class Game;
    class GameObject;
    class Component : IUpdate, IStart, IEnable
    {
    public:
        static std::unordered_map<unsigned int, std::unordered_map<unsigned int, Component *>> GameObjectComponentTypeMap;
        Component(int componentType);
        virtual void Start() override;
        virtual void Update() override;
        virtual void OnComponentAdd(GameObject &parent);
        virtual ~Component();
        inline GameObject *Parent() { return _parent; }
        inline bool IsEnabled() const override { return _enabled; }
        void Enabled(bool isEnabled) override;
        void OnEnabled() override;
        void OnDisabled() override;

    protected:
        Game& GetGame();
        GameObject *_parent;
        unsigned int _componentType;
        bool _enabled;
    };

}