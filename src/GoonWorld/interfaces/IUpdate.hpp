#pragma once
namespace GoonWorld
{
    class IUpdate
    {
    public:
        virtual void Update() = 0;
        virtual ~IUpdate() {}
    };

}