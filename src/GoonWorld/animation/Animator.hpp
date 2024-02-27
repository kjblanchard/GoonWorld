#pragma once
#include <string>
#include <GoonWorld/models/AsepriteDocument.hpp>
#include <GoonWorld/animation/Animation.hpp>
namespace GoonWorld
{
    class Animator
    {
    public:
        static Animator *GetAnimator(std::string filepath);
        Animation *GetAnimation(std::string name);

    private:
        Animator(std::string filepath);
        AsepriteDocument _loadedDocument;
        std::vector<Animation> _animations;
        std::string _defaultAnimation;
        static std::unordered_map<std::string, Animator *> _loadedAnimators;
        friend class AnimationComponent;
    };
}
