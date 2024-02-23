#pragma once
namespace GoonWorld
{
    class IStart
    {
    public:
        virtual void Start() const = 0;
        virtual ~IStart() {}
    };

}