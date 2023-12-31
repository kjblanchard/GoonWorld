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

    public LocationComponent Location => _locationComponent;

    protected LocationComponent _locationComponent;
    protected TagComponent _tagComponent;
    protected ScriptComponent _scriptComponent;
    public virtual void Update(IntPtr input)
    {
        Console.WriteLine($"Update from the gameobject with location {_locationComponent.X}: {_locationComponent.Y}");
        return;
    }

    public GameObject()
    {
        _entity = ECS.NewEntity();
        _tagComponent = new TagComponent("Hello", "nou");
        _locationComponent = new LocationComponent() { X = 20, Y = 30 };
        _scriptComponent = new ScriptComponent(Update);
        AddComponent(_locationComponent, _tagComponent, _scriptComponent);
        AddComponent(_tagComponent);
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