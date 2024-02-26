#pragma once
#include <string>
#include <GoonWorld/base/Component.hpp>
struct SDL_Rect;
namespace GoonWorld
{
    class Animator;
    class Animation;
    class AsepriteDocument;
    class AnimationComponent : public Component
    {
    public:
        AnimationComponent();
        float AnimationSpeed;
        SDL_Rect *SpriteImageRect;

    private:
        std::string _currentAnimationString;
        Animator *_animator;
        Animation *_currentAnimation;
        AsepriteDocument *_currentAnimationDocument;
        double _secondsThisFrame;
        int currentFrame;
    };
}