#pragma once
#include <string>
namespace GoonWorld
{
    struct AnimationTransition
    {
        bool *Condition;
        bool ConditionMatch;
        std::string CurrentAnimation;
        std::string NextAnimation;
    };

}