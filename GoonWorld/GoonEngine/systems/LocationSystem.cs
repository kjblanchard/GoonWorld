
namespace GoonEngine;

public class LocationSystem : System
{
    public const int LocationType = 1;
    public LocationSystem() : base(LocationType)
    {
        RegisterInECS();
    }

    protected override void ComponentUpdate(IntPtr component)
    {
        var locationComponent = ECS.Component.GetComponentOfType<Models.LocationComponent>(component);
        Console.WriteLine($"This component has location x: {locationComponent.x} y: {locationComponent.y}");
    }
}