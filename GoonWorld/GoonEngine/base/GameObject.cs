using GoonEngine.Objects;

namespace GoonEngine;

abstract class GameObject : IStart, IUpdate
{
    public static Dictionary<uint, GameObject> EntityToGameObjectDictionary = new();
    public static List<GameObject> UpdateGameObjects = new();
    public static List<Player> DrawGameObjects = new();
    private static uint _numGameObjects;
    public static Api.Engine.DrawUpdateDelegate DrawFunc = Draw;
    protected PhysicsComponent? _physicsComponent;
    public static TimeSpan DeltaTime;
    public uint Id => _id;
    private uint _id;
    public bool HasTag(string tag) => _tags.Exists(i => i == tag);
    protected void AddTag(string tag) => _tags.Add(tag);

    private List<string> _tags = new();

    public ref Point Location => ref _location;
    private Point _location;
    public GameObject()
    {
        _id = _numGameObjects++;
        _physicsComponent = null;
    }
    ~GameObject() { }
    public virtual void Update()
    {
        if (_physicsComponent != null && _physicsComponent.Body.GravityEnabled == 1)
        {
            Location.X = (int)Math.Floor(_physicsComponent.Body.BoundingBox.X);
            Location.Y = (int)Math.Floor(_physicsComponent.Body.BoundingBox.Y);
        }
    }

    public static void UpdateAllGameObjects()
    {
        UpdateGameObjects.ForEach(gameobject => gameobject.Update());

    }

    public static void Draw()
    {
        DrawGameObjects.ForEach(gameobject => gameobject.DrawMan());
    }

}

public static class GameObjectTypes
{
    public const uint Player = 1;
    public const uint DeathBox = 2;
    public const uint WinBox = 3;
    public const uint Enemy = 4;
}