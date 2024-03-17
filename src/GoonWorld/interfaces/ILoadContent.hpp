#pragma once
namespace GoonWorld
{
    class ILoadContent
    {
    public:
        virtual void Load() = 0;
        virtual void Unload(bool isVisible) = 0;
        virtual ~ILoadContent() {}
    };

}