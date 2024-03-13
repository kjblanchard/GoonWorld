#pragma once
namespace GoonWorld
{
    struct Event
    {
        void *object;
        void *eventArgs;
        int eventType;
    };

}