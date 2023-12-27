namespace GoonEngine;

/// <summary>
/// Gameobject class should be every entity that is spawned,
/// and can have components added to them.
/// </summary>
class GameObject : IStart, IUpdate
{
    /// <summary>
    /// The entity ID is the ID in the ECS system
    /// </summary>
    protected int _entityId;

    GameObject(){}
    ~GameObject(){}

}