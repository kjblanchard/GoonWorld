#pragma once
#include <map>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
namespace GoonWorld
{
    class GameObject;
    class Component : IUpdate, IStart
    {
    public:
        static std::unordered_map<unsigned int, std::unordered_map<unsigned int, Component *>> GameObjectComponentTypeMap;
        Component(int componentType);
        virtual void Start() const override;
        virtual void Update() const override;
        virtual void OnComponentAdd(GameObject &parent);
        virtual ~Component();
        inline GameObject &Parent() { return *_parent; }

    protected:
        GameObject *_parent;
        unsigned int _componentType;
    };

}