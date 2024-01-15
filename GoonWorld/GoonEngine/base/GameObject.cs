using GoonEngine.Components;
namespace GoonEngine;

public abstract class GameObject : IStart, IUpdate
{
    public static Dictionary<uint, GameObject> EntityToGameObjectDictionary = new();
    public static List<GameObject> UpdateGameObjects = new();
    public static List<DrawComponent> DrawGameObjects = new();
    private static uint _numGameObjects;
    protected PhysicsComponent? _physicsComponent;
    public static TimeSpan DeltaTime;
    public uint Id => _id;
    private uint _id;
    public bool HasTag(string tag) => _tags.Contains(tag);
    protected void AddTag(string tag) => _tags.Add(tag);

    private List<string> _tags = new();
    private List<Component> _components = new();
    public void AddComponent(Component component)
    {
        component.OnComponentAdd(this);
        _components.Add(component);
    }
    public void AddComponent(params Component[] components)
    {
        foreach (var component in components)
        {
            AddComponent(component);
        }
    }
    public T AddComponentOfType<T>() where T : Component, new()
    {
        var newComp = new T();
        AddComponent(newComp);
        return newComp;
    }
    public T? GetComponentOfType<T>() where T : Component
    {
        foreach (var component in _components)
        {
            if (component is T matchingComponent)
            {
                return matchingComponent;
            }
        }
        return null;
    }

    public ref Point Location => ref _location;
    private Point _location;
    public GameObject()
    {
        _id = _numGameObjects++;
        _physicsComponent = null;
        UpdateGameObjects.Add(this);
    }
    ~GameObject() { }
    public virtual void Update()
    {
        if (_physicsComponent != null && _physicsComponent.GravityEnabled)
        {
            Location.X = (int)Math.Floor(_physicsComponent.BoundingBox.X);
            Location.Y = (int)Math.Floor(_physicsComponent.BoundingBox.Y);
        }
    }

    public static void UpdateAllGameObjects()
    {
        UpdateGameObjects.ForEach(gameobject => gameobject.Update());
    }

    public static void DrawAllGameObjects()
    {
        DrawGameObjects.ForEach(drawComponent => drawComponent.Draw());
    }

}

public static class GameObjectTypes
{
    public const uint Player = 1;
    public const uint DeathBox = 2;
    public const uint WinBox = 3;
    public const uint Enemy = 4;
}