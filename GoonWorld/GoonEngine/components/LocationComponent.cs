namespace GoonEngine;
public class LocationComponent : Component
{
    public LocationComponent(int x = 0, int y = 0)
    {
        _componentDataPointer = Api.Components.LocationComponent.gnLocationComponentNew();
        Location.x = x;
        Location.y = y;
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.LOCATION_COMPONENT, ComponentPtr );
    }

    public unsafe LocationComponent()
    {
        _componentDataPointer = Api.Components.LocationComponent.gnLocationComponentNew();
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.LOCATION_COMPONENT, ComponentPtr );
    }

    public unsafe ref Models.LocationComponent Location => ref *(Models.LocationComponent*)_componentDataPointer;
}