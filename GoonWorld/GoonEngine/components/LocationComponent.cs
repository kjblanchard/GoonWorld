namespace GoonEngine;
public class LocationComponent : Component
{
    private IntPtr _bodyPtr = IntPtr.Zero;
    public LocationComponent(int x = 0, int y = 0)
    {
        _componentDataPointer = Api.Components.LocationComponent.gnLocationComponentNew();
        X = x;
        Y = y;
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.LOCATION_COMPONENT, ComponentPtr );
    }

    public unsafe LocationComponent(IntPtr bodyPtr)
    {
        _componentDataPointer = Api.Components.LocationComponent.gnLocationComponentNew();
        var thing = (Models.Body*)bodyPtr;
        // var thing2 = (Models.Component*)bodyPtr;
        // X = (int)thing->BoundingBox.x;
        // Y = (int)thing->BoundingBox.y;
        _bodyPtr = bodyPtr;
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.LOCATION_COMPONENT, ComponentPtr );
    }
    public unsafe int X
    {
        get => Api.Components.LocationComponent.gnLocationComponentGetX(ComponentPtr);
        // get => _bodyPtr == IntPtr.Zero ? (int)((Models.Body*)_bodyPtr)->BoundingBox.x : Api.Components.LocationComponent.gnLocationComponentGetX(ComponentPtr);
        set => Api.Components.LocationComponent.gnLocationComponentSetX(ComponentPtr, value);
    }
    public unsafe int Y
    {
        get => Api.Components.LocationComponent.gnLocationComponentGetY(ComponentPtr);
        // get => _bodyPtr == IntPtr.Zero ? (int)((Models.Body*)_bodyPtr)->BoundingBox.y : Api.Components.LocationComponent.gnLocationComponentGetX(ComponentPtr);
        set => Api.Components.LocationComponent.gnLocationComponentSetY(ComponentPtr, value);
    }
}