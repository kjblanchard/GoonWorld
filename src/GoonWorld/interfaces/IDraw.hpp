#pragma once
namespace GoonWorld
{
    class IDraw
    {
    public:
        virtual void Draw() = 0;
        virtual ~IDraw() {}
    };

}