namespace GoonEngine;
public class DrawComponent : Component
{
    public DrawComponent(LocationComponent locationComponent)
    {
        _componentDataPointer = Api.Components.DrawComponent.geDrawComponentNew(locationComponent.ComponentPtr);
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.DRAW_COMPONENT, ComponentPtr);
    }
    public unsafe int Width
    {
        get => ((Models.DrawComponent*)_componentDataPointer)->sizeX;
        set => ((Models.DrawComponent*)_componentDataPointer)->sizeX = value;


    }
    public unsafe int Height
    {
        get => ((Models.DrawComponent*)_componentDataPointer)->sizeY;
        set => ((Models.DrawComponent*)_componentDataPointer)->sizeY = value;
    }

}