using GoonEngine.Content;
using GoonEngine.Interfaces;
using GoonEngine.Models;

namespace GoonEngine.Components;
public class AnimationComponent<T> : Component, IDraw where T : GameObject
{
    public string CurrentAnimation;
    private Animator<T> _animator;
    private Animation<T> _currentAnimation;
    private AsepriteDocument _currentAnimationDocument;
    public float _animationSpeed = 1.0f;
    private double _secondsThisFrame = 0;
    private int currentFrame = 0;
    public Rect SpriteImageRect;
    public bool Mirror { get; set; } = false;
    public Point Offset;
    public byte SizeMultiplier = 1;
    public Rect DrawImageRect => new Rect(Parent.Location.X + Offset.X, Parent.Location.Y + Offset.Y, SpriteImageRect.Width * SizeMultiplier, SpriteImageRect.Height * SizeMultiplier);
    public bool Visible { get; set; } = true;


    public AnimationComponent(Animator<T> animator)
    {
        CurrentAnimation = animator.DefaultAnimation;
        _currentAnimation = animator.Animations[CurrentAnimation];
        _animator = animator;
        _currentAnimationDocument = _animator.BaseDocument;
    }

    public override void Update()
    {
        // Check to see if we should change to a different animation.
        if (_animator.CheckAnimationState(this) is AnimatorTransitionArgs<T> thing)
        {
            ChangeAnimation(thing);
        }
        _secondsThisFrame += GameObject.DeltaTime.TotalMilliseconds * _animationSpeed;
        // Check to see if we should change to the next frame
        var frameSeconds = _currentAnimationDocument.frames[currentFrame].duration;
        if (_secondsThisFrame >= _currentAnimationDocument.frames[currentFrame].duration)
        {
            _secondsThisFrame -= frameSeconds;
            currentFrame = _currentAnimation.Looping ? (currentFrame + 1 <= _currentAnimation.EndFrame) ? currentFrame + 1 : _currentAnimation.StartFrame : currentFrame;
            SpriteImageRect = new Rect(
                _currentAnimationDocument.frames[currentFrame].frame.x,
                _currentAnimationDocument.frames[currentFrame].frame.y,
                _currentAnimationDocument.frames[currentFrame].frame.w,
                _currentAnimationDocument.frames[currentFrame].frame.h
            );
        }
    }

    private void ChangeAnimation(AnimatorTransitionArgs<T> args)
    {
        // Debug.InfoMessage($"I should be switching to {args.Animation.Name}");
        CurrentAnimation = args.Animation.Name;
        _currentAnimation = args.Animation;
        _currentAnimationDocument = args.Document;
        currentFrame = _currentAnimation.StartFrame;
        SpriteImageRect = new Rect(
            _currentAnimationDocument.frames[currentFrame].frame.x,
            _currentAnimationDocument.frames[currentFrame].frame.y,
            _currentAnimationDocument.frames[currentFrame].frame.w,
            _currentAnimationDocument.frames[currentFrame].frame.h
        );
    }

    public void Draw()
    {
        if (!Enabled || !Visible || _currentAnimation == null)
            return;
        var drawRect = DrawImageRect;
        Api.Rendering.DrawTexture(_currentAnimation.Image.LoadedImagePtr, ref SpriteImageRect, ref drawRect);
    }
    public override void OnComponentAdd(GameObject parent)
    {
        base.OnComponentAdd(parent);
        GameObject.CurrentDrawComponents.Add(this);
    }

}

public class Animation<T>
{
    public string Name;
    public bool Looping;
    public Image? Image;
    public int StartFrame;
    public int EndFrame;
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