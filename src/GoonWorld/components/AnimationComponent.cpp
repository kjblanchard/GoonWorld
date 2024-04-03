#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/Animator.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonWorld/core/Camera.hpp>
// #include <GoonEngine/SdlSurface.h>

// temp
// #include <GoonEngine/Shader.h>
// #include <GoonEngine/Sprite.h>
#include <GoonEngine/color.h>
using namespace GoonWorld;

// extern geShader *shader;

AnimationComponent::AnimationComponent(std::string animator)
    : Component((int)ComponentTypes::Animation), AnimationSpeed(1.0), _animator(Animator::GetAnimator(animator)), _visible(true)
{
    _currentAnimationDocument = &_animator->_loadedDocument;
    ChangeAnimation(_animator->_defaultAnimation);
    // _sprite = geSpriteRendererNew(shader);
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
        SpriteImageRect = geRectangle{
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
    SpriteImageRect = geRectangle{
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
    SpriteImageRect = geRectangle{
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
    // geDrawTextureWithCameraOffset(_currentAnimation->Image, &SpriteImageRect, &rect, Mirror);
    GetGame().SpritebatchDraw(_currentAnimation->Image->GetTexture(), rect,
                              0, geColor{1, 1, 1, 1}, SpriteImageRect, Mirror);
    // GetGame().SpritebatchDraw(_currentAnimation->Image->GetTexture(), vec2{(float)rect.x, (float)rect.y},
    //                           vec2{(float)rect.w, (float)rect.h}, 0, vec4{1, 1, 1, 1}, vec2{(float)SpriteImageRect.x, (float)SpriteImageRect.y},
    //                           vec2{(float)SpriteImageRect.w, (float)SpriteImageRect.h}, Mirror);
    // geSpriteRendererDraw(_sprite, _currentAnimation->Image->GetTexture(), vec2{(float)rect.x, (float)rect.y},
    //                      vec2{(float)rect.w, (float)rect.h}, 0, vec4{1, 1, 1, 1}, vec2{(float)SpriteImageRect.x, (float)SpriteImageRect.y},
    //                      vec2{(float)SpriteImageRect.w, (float)SpriteImageRect.h}, Mirror);
}

void AnimationComponent::OnComponentAdd(GameObject &parent)
{
    Component::OnComponentAdd(parent);
    GetGame().AddDrawObject(this);
}

geRectangle AnimationComponent::GetDrawRect()
{
    return geRectangle{Parent()->Location().x + _offset.x, Parent()->Location().y + _offset.y, SpriteImageRect.w * SizeMultiplier, SpriteImageRect.h * SizeMultiplier};
}

void AnimationComponent::AddTransition(std::string from, std::string to, bool matchCondition, bool *matchRef)
{
    _transitions.push_back(std::make_unique<AnimationTransition>(from, to, matchCondition, matchRef));
}

void AnimationComponent::ChangeLayer(int layer)
{
    GetGame().ChangeDrawObjectLayer(this, layer);
    _layer = layer;
}