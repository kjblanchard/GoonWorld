#pragma once
namespace GoonWorld
{
    class ILoadContent
    {
    public:
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual ~ILoadContent() {}
    };

}