#pragma once
#include <string>
#include <GoonWorld/models/AsepriteDocument.hpp>
#include <GoonWorld/animation/Animation.hpp>
namespace GoonWorld
{
    class Animator
    {
    public:
        static std::unordered_map<std::string, Animator *> LoadedAnimators;
        Animator(std::string filepath);

    private:
        AsepriteDocument _loadedDocument;
        std::vector<Animation> _animations;
        std::string _defaultAnimation;
    };
}
