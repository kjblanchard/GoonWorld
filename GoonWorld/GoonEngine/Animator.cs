using System.Text.Json;
using GoonEngine.Models;
using GoonEngine.Components;

namespace GoonEngine;
public class Animator<T> where T : GameObject
{
    private AsepriteDocument _loadedDocument;
    private Dictionary<string, Animation<T>> Animations = new();
    public void LoadAnimationFile(string filepath)
    {
        var fullPath = $"assets/img/{filepath}.json";
        string jsonContent = File.ReadAllText(fullPath);
        _loadedDocument = JsonSerializer.Deserialize<AsepriteDocument>(jsonContent);
        // This should all be on the "Animator" for a class like "Player" as it will be shared between all
        // Load Image
        // For each frametag
        // Grab name and from/to
        // these should be linked to an image, so that when we have big / fireball we can quickly switch

    }
    public void AddAnimation(Animation<T> animation)
    {
        Animations[animation.Name] = animation;
    }
    public string? CheckAnimationState(AnimationComponent<T> component)
    {
        if (!Animations.TryGetValue(component.CurrentAnimation, out var animations))
            return null;
        foreach (var transition in animations.Transitions)
        {
            if (transition.TransitionCondition((T)component.Parent))
            {
                return transition.TransitionAnimationTag;
            }
        }
        return null;
    }

}