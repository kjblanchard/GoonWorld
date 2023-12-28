namespace GoonEngine;

/// <summary>
/// Gameobject class should be every entity that is spawned,
/// and can have components added to them.
/// </summary>
class GameObject : IStart, IUpdate
{
    public const int TAG_COMPONENT = 0;
    public const int LOCATION_COMPONENT = 1;
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
        _entity = ECS.NewEntity();
        _locationComponent = new LocationComponent() { X = 20, Y = 30 };
        _tagComponent = new TagComponent("Hello", "nou");
        var hasTag = _tagComponent.HasTag("nou");
        AddComponent(_locationComponent, _tagComponent);
        // var thing = ECS.Component.GetComponentOfType<Models.LocationComponent>( _locationComponent.ECSComponentPtr);
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