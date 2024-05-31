#pragma once
#include <GoonWorld/base/UiObject.hpp>
#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>
#include <GoonEngine/color.h>

namespace GoonWorld
{

    class BoxUi : public UiObject
    {
    public:
        BoxUi(geRectangle rect);
        void Draw() override;
        inline void SetLocation(Point location)
        {
            _location.x = location.x;
            _location.y = location.y;
        }
        inline void SetFilled(bool isFilled) { _filled = isFilled; }
        inline void SetColor(geColor color) { _drawColor = color; }
        inline void SetTransparency(int alpha) { _drawColor.A = alpha; }

    private:
        geRectangle _location;
        bool _filled = true;
        geColor _drawColor;
    };
}