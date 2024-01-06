using System.Security.Cryptography.X509Certificates;

namespace GoonEngine;

/// <summary>
/// Gameobject class should be every entity that is spawned,
/// and can have components added to them.
/// </summary>
class GameObject : IStart, IUpdate
{

    public static Dictionary<IntPtr, GameObject> EntityToGameObjectDictionary = new();

    /// <summary>
    /// The entity ID is the ID in the ECS system
    /// </summary>
    public uint Type => _type;
    public IntPtr Entity => _entity;
    private IntPtr _entity;
    private uint _type;

    public ref Models.LocationComponent Location => ref _locationComponent.Location;
    public TagComponent Tag => _tagComponent;

    protected LocationComponent _locationComponent;
    protected TagComponent _tagComponent;
    protected ScriptComponent _scriptComponent;
    protected PhysicsComponent? _physicsComponent;
    public static TimeSpan DeltaTime;
    public virtual void Update()
    {
        if (_physicsComponent != null)
        {
            Location.x = (int)Math.Round(_physicsComponent.Body.BoundingBox.x);
            Location.y = (int)Math.Round(_physicsComponent.Body.BoundingBox.y);
        }
    }

    public GameObject()
    {
        _entity = ECS.NewEntity();
        _tagComponent = new TagComponent();
        _locationComponent = new LocationComponent();
        _scriptComponent = new ScriptComponent(Update);
        _physicsComponent = null;
        AddComponent(_locationComponent, _tagComponent, _scriptComponent);
        EntityToGameObjectDictionary[_entity] = this;
    }
    public void AddComponent(params Component[] components)
    {
        foreach (var component in components)
        {
            ECS.Entity.geEntityAddComponent(_entity, component.ECSComponentPtr);
        }
    }

    public bool HasComponent(uint type) => ECS.Entity.geEntityHasComponent(_entity, type);
    ~GameObject() { }

}

public static class GameObjectTypes
{
    public const uint Player = 1;
    public const uint DeathBox = 2;
    public const uint WinBox = 3;
    public const uint Enemy = 4;
}