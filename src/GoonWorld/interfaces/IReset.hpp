#pragma once
namespace GoonWorld
{
    class IReset
    {
    public:
        virtual void Reset() = 0;
        virtual ~IReset() {}
    };

}