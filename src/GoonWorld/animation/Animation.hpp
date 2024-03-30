#pragma once
#include <string>
#include <vector>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/content/Image.hpp>
#include <GoonEngine/Texture2D.h>

// #include <SDL2/SDL_render.h>
// struct SDL_Texture;
namespace GoonWorld
{
    class Animation
    {
    public:
        std::string Name;
        bool Looping;
        Image *Image;
        int StartFrame, EndFrame;
        std::vector<AnimationTransition> Transitions;
    };
}