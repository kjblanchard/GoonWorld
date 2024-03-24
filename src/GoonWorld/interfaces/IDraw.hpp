#pragma once
namespace GoonWorld
{
    class IDraw
    {
    public:
        virtual void Draw(double accum) = 0;
        virtual void Visible(bool isVisible) = 0;
        virtual bool IsVisible() = 0;
        virtual ~IDraw() {}
    };

}