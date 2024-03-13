#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/Animator.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/SdlSurface.h>
using namespace GoonWorld;

AnimationComponent::AnimationComponent(std::string animator)
    : Component((int)ComponentTypes::Animation), _animator(Animator::GetAnimator(animator)), AnimationSpeed(1.0), _visible(true)
{
    _currentAnimationDocument = &_animator->_loadedDocument;
    ChangeAnimation(_animator->_defaultAnimation);
}

AnimationComponent::AnimationComponent(std::string animator, Point offset)
    : AnimationComponent(animator)
{
    _visible = true;
    _offset = offset;
}
void AnimationComponent::Update()
{
    for (auto &transition : _transitions)
    {
        if (transition->CurrentAnimation != _currentAnimationString)
            continue;
        if (*transition->Condition == transition->ConditionMatch)
        {
            ChangeAnimation(transition->NextAnimation);
            break;
        }
    }
    if (_currentAnimation->StartFrame == _currentAnimation->EndFrame)
        return;
    _secondsThisFrame += GameObject::DeltaTime.GetTotalMilliseconds() * AnimationSpeed;
    auto frameSeconds = _currentAnimationDocument->frames[_currentFrame].duration;
    if (_secondsThisFrame >= frameSeconds)
    {
        _secondsThisFrame -= frameSeconds;
        _currentFrame = _currentAnimation->Looping ? (_currentFrame + 1 <= _currentAnimation->EndFrame) ? _currentFrame + 1 : _currentAnimation->StartFrame : _currentFrame;
        SpriteImageRect = SDL_Rect{
            _currentAnimationDocument->frames[_currentFrame].frame.x,
            _currentAnimationDocument->frames[_currentFrame].frame.y,
            _currentAnimationDocument->frames[_currentFrame].frame.w,
            _currentAnimationDocument->frames[_currentFrame].frame.h,
        };
    }
}
void AnimationComponent::ChangeAnimation(std::string &anim, int frameInAnim)
{
    auto newAnimation = _animator->GetAnimation(anim);
    if (!newAnimation)
        return;
    _currentAnimation = newAnimation;
    _currentAnimationString = anim;
    _currentFrame = frameInAnim;
    SpriteImageRect = SDL_Rect{
        _currentAnimationDocument->frames[_currentFrame].frame.x,
        _currentAnimationDocument->frames[_currentFrame].frame.y,
        _currentAnimationDocument->frames[_currentFrame].frame.w,
        _currentAnimationDocument->frames[_currentFrame].frame.h,
    };
}
void AnimationComponent::ChangeAnimation(std::string &nextAnim)
{
    auto newAnimation = _animator->GetAnimation(nextAnim);
    if (!newAnimation)
        return;
    _currentAnimationString = nextAnim;
    _currentAnimation = newAnimation;
    _currentFrame = _currentAnimation->StartFrame;
    _secondsThisFrame = 0;
    SpriteImageRect = SDL_Rect{
        _currentAnimationDocument->frames[_currentFrame].frame.x,
        _currentAnimationDocument->frames[_currentFrame].frame.y,
        _currentAnimationDocument->frames[_currentFrame].frame.w,
        _currentAnimationDocument->frames[_currentFrame].frame.h,
    };
}
void AnimationComponent::Draw()
{
    if (!IsEnabled() || _currentAnimation == nullptr)
        return;
    auto rect = GetDrawRect();
    DrawTexture(_currentAnimation->Image, &SpriteImageRect, &rect, Mirror);
}

void AnimationComponent::OnComponentAdd(GameObject &parent)
{
    Component::OnComponentAdd(parent);
    GetGame().AddDrawObject(this);
}

SDL_Rect AnimationComponent::GetDrawRect()
{
    return SDL_Rect{Parent()->Location().x + _offset.x, Parent()->Location().y + _offset.y, SpriteImageRect.w * SizeMultiplier, SpriteImageRect.h * SizeMultiplier};
}

void AnimationComponent::AddTransition(std::string from, std::string to, bool matchCondition, bool *matchRef)
{
    _transitions.push_back(std::make_unique<AnimationTransition>(from, to, matchCondition, matchRef));
}