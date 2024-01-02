namespace GoonEngine;

public class ScriptSystem : System
{
    public ScriptSystem() : base(ComponentTypes.SCRIPT_COMPONENT)
    {
        RegisterInECS();
    }

    protected override void ComponentUpdate(IntPtr ecsComponentPtr)
    {
        GameObject.DeltaTime = ElapsedTime;
        var scriptComponent = ECS.Component.GetComponentOfType<Models.ScriptComponent>(ecsComponentPtr);
        scriptComponent.UpdateFunc();
    }
}