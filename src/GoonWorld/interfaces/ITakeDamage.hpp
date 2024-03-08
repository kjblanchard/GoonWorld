#pragma once
namespace GoonWorld
{
    class ITakeDamage
    {
    public:
        virtual void TakeDamage() = 0;
        virtual ~ITakeDamage() {}
    };

}