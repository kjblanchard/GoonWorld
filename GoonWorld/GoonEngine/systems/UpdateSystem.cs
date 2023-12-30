namespace GoonEngine;

public class ScriptSystem : System
{
    public ScriptSystem() : base(ComponentTypes.SCRIPT_COMPONENT)
    {
        RegisterInECS();
    }

    protected override void ComponentUpdate(IntPtr component)
    {
        var scriptComponent = ECS.Component.GetComponentOfType<Models.ScriptComponent>(component);
        scriptComponent.UpdateFunc(IntPtr.Zero);
    }
}