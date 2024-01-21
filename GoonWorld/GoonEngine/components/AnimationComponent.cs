using GoonEngine.Models;

namespace GoonEngine.Components;
public class AnimationComponent<T> : Component where T: GameObject
{
    public string CurrentAnimation;
    private Animator<T> _animator;

    public AnimationComponent(string filepath, Animator<T> animator)
    {
        _animator = animator;
        CurrentAnimation = "idle";
    }

    public override void Update()
    {
        if(_animator.CheckAnimationState(this) is var thing && thing != null)
        {
            Console.WriteLine($"I should switch to animation {thing}");
        }
    }

}
// using System;
// using Microsoft.Xna.Framework;
// using MonoGame.Aseprite.Documents;
// using SupergoonDashCrossPlatform.SupergoonEngine.Animation;
// using SupergoonDashCrossPlatform.SupergoonEngine.Core;

// namespace SupergoonDashCrossPlatform.SupergoonEngine.Components;

// public class AnimationComponent : Component
// {
//     private SpriteComponent _spriteComponent;
//     private AsepriteDocument _asepriteDocument;
//     private Animator _animator = new Animator();
//     private string _currentAnimationTag;

//     public float _animationSpeed = 1.0f;

//     private Point _texturePointToDisplay = Point.Zero;


//     private double _secondsThisFrame = 0;
//     private int currentFrame = 0;
//     private int endingFrame = 0;

//     public AnimationComponent(GameObject parent, SpriteComponent spriteComponent, AsepriteDocument asepriteDocument) :
//         base(parent)
//     {
//         _spriteComponent = spriteComponent;
//         _asepriteDocument = asepriteDocument;
//         _animator.AnimationTransitionEvent += OnAnimationChange;
//     }

//     public void ChangeAnimation(string animTag)
//     {
//         _animator.ChangeAnimation(animTag);
//     }

//     public override void Update(GameTime gameTime)
//     {
//         base.Update(gameTime);
//         _animator.Update(gameTime);
//         // _secondsThisFrame += gameTime.ElapsedGameTime.TotalSeconds;
//         _secondsThisFrame += gameTime.ElapsedGameTime.TotalSeconds * _animationSpeed;
//         if (_animator.CurrentAnimation.Looping && _secondsThisFrame >= _asepriteDocument.Frames[currentFrame].Duration)
//         {
//             _secondsThisFrame -= _asepriteDocument.Frames[currentFrame].Duration;
//             var newFrame = currentFrame + 1;
//             if (newFrame >= endingFrame)
//                 newFrame = _asepriteDocument.Tags[_currentAnimationTag].From;
//             currentFrame = newFrame;
//             _texturePointToDisplay = new Point(_asepriteDocument.Frames[currentFrame].X,
//                 _asepriteDocument.Frames[currentFrame].Y);
//         }

//         _spriteComponent.UpdateFromAnimationComponent(_asepriteDocument.Texture, _texturePointToDisplay);
//     }

//     public void AddAnimationTransition(params AnimationProperties[] transition)
//     {
//         _animator.AddAnimationTransition(transition);
//     }

//     public void OnAnimationChange()
//     {
//         _secondsThisFrame = 0;
//         _currentAnimationTag = _animator.CurrentAnimationTag;
//         currentFrame = _asepriteDocument.Tags[_currentAnimationTag].From;
//         endingFrame = _asepriteDocument.Tags[_currentAnimationTag].To;
//         _texturePointToDisplay = new Point(_asepriteDocument.Frames[currentFrame].X,
//             _asepriteDocument.Frames[currentFrame].Y);
//     }

//     public void SetAnimationEntryTransmission(AnimationTransition transition)
//     {
//         _animator.EntryTransition = transition;
//     }
// }




public class Animation<T>
{
    public string Name;
    public bool Looping;
    public List<AnimationTransition<T>> Transitions = new();
    public Animation(string name, bool looping = true)
    {
        Name = name;
        Looping = looping;
    }
}

public class AnimationTransition<T>
{
    public string TransitionAnimationTag;
    public Func<T, bool> TransitionCondition;

    public AnimationTransition(string transitionAnimationTag, Func<T, bool> transitionCondition)
    {
        TransitionAnimationTag = transitionAnimationTag;
        TransitionCondition = transitionCondition;
    }
}