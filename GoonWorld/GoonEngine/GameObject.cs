using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics.X86;

namespace GoonEngine;

/// <summary>
/// Gameobject class should be every entity that is spawned,
/// and can have components added to them.
/// </summary>
class GameObject : IStart, IUpdate
{
    public const int TAG_COMPONENT = 0;
    public const int LOCATION_COMPONENT = 1;
    public static IntPtr GameObjectContext = IntPtr.Zero;
    public static Dictionary<IntPtr, GameObject> EntityToGameObjectDictionary = new();

    /// <summary>
    /// The entity ID is the ID in the ECS system
    /// </summary>
    public IntPtr Entity => _entity;
    private IntPtr _entity;

    protected LocationComponent _locationComponent;
    protected TagComponent _tagComponent;

    public GameObject()
    {
        if (GameObjectContext == IntPtr.Zero)
        {
            GameObjectContext = ECS.geContextNew();
        }
        _entity = ECS.geContextEntityNew(GameObjectContext);
        _locationComponent = new LocationComponent(GameObjectContext) { X = 20, Y = 30 };
        _tagComponent = new TagComponent(GameObjectContext);
        var tag = "Hello";
        _tagComponent.AddTag(tag);
        var hasTag = _tagComponent.HasTag(tag);
        AddComponent(_locationComponent, _tagComponent);
        EntityToGameObjectDictionary[_entity] = this;
    }
    // public void AddComponent(IComponent component) => ECS.geEntityAddComponent(GameObjectContext, component.Ptr);
    // public void AddComponent(params IntPtr[] components)
    // {
    //     foreach(var component in components)
    //     {
    //         ECS.geEntityAddComponent(_entity, component);
    //     }

    // }
    public void AddComponent(params IComponent[] components)
    {
        foreach(var component in components)
        {
            ECS.geEntityAddComponent(_entity, component.ECSComponentPtr);
        }

    }

    public bool HasComponent(uint type) => ECS.geEntityHasComponent(_entity, type);
    ~GameObject() { }


}