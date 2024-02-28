#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/Animator.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/SdlSurface.h>
#include <chrono>
using namespace GoonWorld;

AnimationComponent::AnimationComponent(std::string animator)
    : Component((int)ComponentTypes::Animation), _animator(Animator::GetAnimator(animator)), AnimationSpeed(1.0)
{
    _currentAnimationDocument = &_animator->_loadedDocument;
    ChangeAnimation(_animator->_defaultAnimation);
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
    // auto msDouble = std::chrono::duration_cast<std::chrono::milliseconds>(GameObject::DeltaTime);
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
void AnimationComponent::ChangeAnimation(std::string &nextAnim)
{
    _currentAnimationString = nextAnim;
    _currentAnimation = _animator->GetAnimation(nextAnim);
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
    Game::Instance()->DrawObjects.push_back(this);
}

SDL_Rect AnimationComponent::GetDrawRect()
{
    return SDL_Rect{Parent()->Location().x + _offset.x, Parent()->Location().y + _offset.y, SpriteImageRect.w * SizeMultiplier, SpriteImageRect.h * SizeMultiplier};
}