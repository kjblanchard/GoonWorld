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
        Animator(std::string filepath);
        ~Animator();
        Animation *GetAnimation(std::string name);

    private:
        AsepriteDocument _loadedDocument;
        std::vector<Animation> _animations;
        std::string _defaultAnimation;
        static std::unordered_map<std::string, std::shared_ptr<Animator>> _loadedAnimators;
        friend class AnimationComponent;
    };
}
