using System.Text.Json;
using GoonEngine.Models;
using GoonEngine.Content;
using GoonEngine.Components;

namespace GoonEngine;
public class Animator<T> where T : GameObject
{
    public AsepriteDocument BaseDocument => _loadedDocument;
    private AsepriteDocument _loadedDocument;
    public string DefaultAnimation;
    public Dictionary<string, Animation<T>> Animations = new();
    public void LoadAnimationFile(string filepath)
    {
        var fullPath = $"assets/img/{filepath}.json";
        string jsonContent = File.ReadAllText(fullPath);
        _loadedDocument = JsonSerializer.Deserialize<AsepriteDocument>(jsonContent);
         if(!Game.Global.Config.Config.Animations.TryGetValue(filepath, out var data))
         {
            return;
         }
         DefaultAnimation = data.Default;
         foreach(var animation in data.animations)
         {
            var newAnim = new Animation<T>(animation.name, animation.looping);
            AddAnimation(newAnim);
            // Animations[animation.name] = newAnim;
         }
    }

    public void AddAnimationTransition(string current, string transition, Func<T, bool> func)
    {
        var animTransition = new AnimationTransition<T>(transition, func);
        Animations[current].Transitions.Add(animTransition);
    }

    public void AddAnimation(Animation<T> animation)
    {
        // animation.Image = Image.LoadImage(GetImagePath(_loadedDocument.meta.image));
        animation.Image = Image.LoadImage(_loadedDocument.meta.image);
        foreach (var transition in _loadedDocument.meta.frameTags)
        {
            if (transition.name == animation.Name)
            {
                {
                    animation.StartFrame = transition.from;
                    animation.EndFrame = transition.to;
                }
            }
        }
        Animations[animation.Name] = animation;
    }

    public AnimatorTransitionArgs<T>? CheckAnimationState(AnimationComponent<T> component)
    {
        if (!Animations.TryGetValue(component.CurrentAnimation, out var animations))
            return null;
        foreach (var transition in animations.Transitions)
        {
            if (transition.TransitionCondition.Invoke((T)component.Parent))
            {
                return new AnimatorTransitionArgs<T> { Animation = Animations[transition.TransitionAnimationTag], Document = _loadedDocument };
            }
        }
        return null;
    }
}

public struct AnimatorTransitionArgs<T>
{
    public Animation<T> Animation;
    public AsepriteDocument Document;
}