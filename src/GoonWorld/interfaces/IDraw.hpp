#pragma once
namespace GoonWorld
{
    class IDraw
    {
    public:
        virtual void Draw() const = 0;
        virtual ~IDraw() {}
    };

}