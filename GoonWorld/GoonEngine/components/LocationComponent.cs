namespace GoonEngine;
public class LocationComponent : Component
{
    public LocationComponent(int x = 0, int y = 0)
    {
        _componentDataPointer = Api.Components.LocationComponent.gnLocationComponentNew();
        X = x;
        Y = y;
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.LOCATION_COMPONENT, ComponentPtr );
    }
    public int X
    {
        get => Api.Components.LocationComponent.gnLocationComponentGetX(ComponentPtr);
        set => Api.Components.LocationComponent.gnLocationComponentSetX(ComponentPtr, value);
    }
    public int Y
    {
        get => Api.Components.LocationComponent.gnLocationComponentGetY(ComponentPtr);
        set => Api.Components.LocationComponent.gnLocationComponentSetY(ComponentPtr, value);
    }
}