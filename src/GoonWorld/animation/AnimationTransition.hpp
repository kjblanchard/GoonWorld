#pragma once
#include <string>
namespace GoonWorld
{
    struct AnimationTransition
    {
        inline AnimationTransition(std::string from, std::string to, bool matchCondition, bool *matchRef)
            : Condition(matchRef), ConditionMatch(matchCondition), CurrentAnimation(from), NextAnimation(to) {}
        bool *Condition;
        bool ConditionMatch;
        std::string CurrentAnimation;
        std::string NextAnimation;
    };

}
