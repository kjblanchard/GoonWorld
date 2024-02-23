#pragma once
namespace GoonWorld
{
    class IUpdate
    {
    public:
        virtual void Update() const = 0;
        virtual ~IUpdate(){}
    };

}