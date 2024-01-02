namespace GoonEngine;
public class DrawSystem : System
{
    Api.Rendering.DrawUpdateDelegate drawDelegate;
    public DrawSystem() : base(ComponentTypes.DRAW_COMPONENT)
    {
        drawDelegate = ComponentForEach;
        Api.Rendering.geContextSetDrawFunc(drawDelegate);
    }

    protected unsafe override void ComponentUpdate(IntPtr component)
    {
        var componentData = (Models.Component*)component;
        Api.Components.DrawComponent.geDrawComponentDraw(componentData->Data);
    }
}