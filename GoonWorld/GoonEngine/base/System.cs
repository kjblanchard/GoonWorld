using System.Runtime.InteropServices;
namespace GoonEngine;

public abstract class System
{
    private int _type;
    public TimeSpan ElapsedTime;
    ECS.System.SystemDelegate deleg;

    public System(int type)
    {
        _type = type;
        deleg = Update;
    }
    public virtual void Start() { }
    public unsafe void Update(ref IntPtr context, int type, IntPtr data)
    {
        long ticks = (long)(((Models.UpdateData*)data)->UpdateTime * TimeSpan.TicksPerMillisecond);
        TimeSpan timeSpan = TimeSpan.FromTicks(ticks);
        ComponentForEach();
    }
    protected virtual void ComponentUpdate(IntPtr ecsComponentPtr) { }
    public void RegisterInECS()
    {
        ECS.System.NewSystem(deleg, _type);
    }
    public unsafe void ComponentForEach()
    {
        var componentsPtr = ECS.GetComponentsOfType(_type);
        var componentCount = ECS.GetNumComponentsOfType(_type);
        if (componentsPtr == IntPtr.Zero)
            return;
        for (int i = 0; i < componentCount; i++)
        {
            ComponentUpdate(Marshal.ReadIntPtr(componentsPtr, i * IntPtr.Size));
        }
    }

}