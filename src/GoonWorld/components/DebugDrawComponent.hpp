#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonEngine/color.h>
#include <GoonEngine/point.h>

namespace GoonWorld
{
    class DebugDrawComponent : public Component, IDraw
    {
    public:
        DebugDrawComponent(Point size);
        void Draw() override;
        void OnComponentAdd(GameObject &parent) override;
        Color DrawColor;
        Point Size;

    private:
    };

}