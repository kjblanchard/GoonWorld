#pragma once
#include <string>
#include <GoonWorld/base/Component.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <SDL2/SDL_rect.h>
#include <GoonEngine/point.h>
namespace GoonWorld
{
    class Animator;
    class Animation;
    class AsepriteDocument;
    class AnimationTransition;
    class AnimationComponent : public Component, IDraw
    {
    public:
        AnimationComponent(std::string animator);
        AnimationComponent(std::string animator, Point offset);
        float AnimationSpeed;
        SDL_Rect SpriteImageRect;
        void Update() override;
        void Draw() override;
        void OnComponentAdd(GameObject &parent) override;
        inline std::pair<int, Animation *> GetCurrentAnimation() const { return {_currentFrame, _currentAnimation}; };
        void ChangeAnimation(std::string &, int frameInAnim);
        void ChangeAnimation(std::string &);
        inline void Offset(Point offset) { _offset = offset; }
        int SizeMultiplier;
        inline void AddTransition(AnimationTransition *transition) { _transitions.push_back(transition); }
        void AddTransition(std::string from, std::string to, bool matchCondition, bool *matchRef);
        bool Mirror = false;

    private:
        SDL_Rect GetDrawRect();
        std::string _currentAnimationString;
        Animator *_animator;
        Animation *_currentAnimation;
        AsepriteDocument *_currentAnimationDocument;
        double _secondsThisFrame;
        int _currentFrame;
        Point _offset = Point{0, 0};
        std::vector<AnimationTransition *> _transitions;
    };
}