
#pragma once
namespace GoonWorld
{
    class IEnable
    {
    public:
        virtual bool IsEnabled() const = 0;
        virtual void Enabled(bool isEnabled) = 0;
        virtual void OnEnabled() = 0;
        virtual void OnDisabled() = 0;
        virtual ~IEnable() {}
    };

}