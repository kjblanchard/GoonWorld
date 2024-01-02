namespace GoonEngine;
using GoonEngine.Models;
public class DrawComponent : Component
{
    public DrawComponent(LocationComponent locationComponent)
    {
        _componentDataPointer = Api.Components.DrawComponent.geDrawComponentNew(locationComponent.ComponentPtr);
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.DRAW_COMPONENT, ComponentPtr);
    }
    public unsafe int Width
    {
        get
        {
            var thing = ((Models.DrawComponent*)_componentDataPointer)->sizeX;
            return thing;

        }
        set
        {
            Api.Components.DrawComponent.geDrawComponentSetWidth(_componentDataPointer, value);
            // var beforeValue = Width;
            // var thing = value;
            // ((Models.DrawComponent*)_componentDataPointer)->sizeX = thing;
        }


    }
    public unsafe int Height
    {
        get => ((Models.DrawComponent*)_componentDataPointer)->sizeY;
        // set => ((Models.DrawComponent*)_componentDataPointer)->sizeY = value;
            set => Api.Components.DrawComponent.geDrawComponentSetHeight(_componentDataPointer, value);
    }

}