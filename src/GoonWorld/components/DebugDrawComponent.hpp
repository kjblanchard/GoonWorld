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
        inline void Visible(bool isVisible) override { _visible = isVisible; }
        inline bool IsVisible() override { return _visible; }
        Color DrawColor;
        Point Size;

    private:
        bool _visible;
    };

}