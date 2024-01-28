using GoonEngine.Interfaces;
namespace GoonEngine;

public abstract class GameObject : IStart, IUpdate
{
    public static List<IUpdate> UpdateGameObjects = new();
    public static List<IDraw> CurrentDebugDrawComponents = new();
    // public static List<SpriteComponent> CurrentDrawComponents = new();
    public static List<IDraw> CurrentDrawComponents = new();
    public static TimeSpan DeltaTime;
    protected static Dictionary<uint, GameObject> EntityToGameObjectDictionary = new();
    private static uint _numGameObjects;
    public uint Id => _id;
    private uint _id;
    public bool HasTag(string tag) => _tags.Contains(tag);
    protected void AddTag(string tag) => _tags.Add(tag);
    private List<string> _tags = new();
    private List<Component> _components = new();
    public ref Point Location => ref _location;
    private Point _location;
    // public bool Enabled => _enabled;
    public bool Enabled
    {
        get => _enabled;
        set
        {
            _enabled = value;
            foreach(var component in _components)
            {
                component.Enabled = value;
            }

        }
    }
    protected bool _enabled = true;

    public GameObject()
    {
        _id = _numGameObjects++;
        UpdateGameObjects.Add(this);
    }
    ~GameObject() { }

    public virtual void Update()
    {
        _components.ForEach(component => component.Update());

    }

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

    public static void UpdateAllGameObjects()
    {
        UpdateGameObjects.ForEach(gameobject =>
        {
            if (gameobject.Enabled)
            {
                gameobject.Update();
            }
        }
        );
    }
    public static void DrawGameObjectBoxes()
    {
        // CurrentDrawComponents.ForEach(spriteComponent => spriteComponent.Draw());
        CurrentDrawComponents.ForEach(
            spriteComponent =>
            {
                if (spriteComponent.Enabled)
                {
                    spriteComponent.Draw();
                }
            }
        );
    }

    public static void DrawGameObjectDebugBoxes()
    {
        CurrentDebugDrawComponents.ForEach(drawComponent => drawComponent.Draw());
    }

}

public static class GameObjectTypes
{
    public const uint Player = 1;
    public const uint DeathBox = 2;
    public const uint WinBox = 3;
    public const uint Enemy = 4;
}