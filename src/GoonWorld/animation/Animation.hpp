#pragma once
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <string>
#include <vector>
struct SDL_Texture;
namespace GoonWorld
{
    struct Animation
    {
        std::string Name;
        bool Looping;
        SDL_Texture *Image;
        int StartFrame, EndFrame;
        std::vector<AnimationTransition> Transitions;
    };
}