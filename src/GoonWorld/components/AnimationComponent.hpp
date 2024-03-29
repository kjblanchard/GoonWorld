#pragma once
#include <string>
#include <GoonWorld/base/Component.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/interfaces/IDraw.hpp>
#include <GoonEngine/point.h>
#include <GoonEngine/rectangle.h>
namespace GoonWorld
{
    class Animator;
    class Animation;
    class AsepriteDocument;
    class AnimationComponent : public Component, public IDraw
    {
    public:
        AnimationComponent(std::string animator);
        AnimationComponent(std::string animator, Point offset);
        float AnimationSpeed;
        geRectangle SpriteImageRect;
        void Update() override;
        void Draw() override;
        void OnComponentAdd(GameObject &parent) override;
        inline std::pair<int, Animation *> GetCurrentAnimation() const { return {_currentFrame, _currentAnimation}; };
        void ChangeAnimation(std::string &, int frameInAnim);
        void ChangeLayer(int layer);
        void ChangeAnimation(std::string &);
        inline void Visible(bool isVisible) override { _visible = isVisible; }
        inline bool IsVisible() override { return _visible; }
        inline void Offset(Point offset) { _offset = offset; }
        int SizeMultiplier = 1;
        inline void AddTransition(AnimationTransition *transition) { _transitions.push_back(std::unique_ptr<AnimationTransition>(transition)); }
        void AddTransition(std::string from, std::string to, bool matchCondition, bool *matchRef);
        bool Mirror = false;

    private:
        geRectangle GetDrawRect();
        std::string _currentAnimationString;
        Animator *_animator;
        Animation *_currentAnimation;
        AsepriteDocument *_currentAnimationDocument;
        double _secondsThisFrame;
        int _currentFrame;
        int _layer;
        Point _offset = Point{0, 0};
        std::vector<std::unique_ptr<AnimationTransition>> _transitions;
        bool _visible;
    };
}