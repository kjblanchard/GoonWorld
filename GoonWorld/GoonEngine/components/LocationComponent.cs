using System.Runtime.InteropServices;
namespace GoonEngine;
public class LocationComponent : Component
{
    public LocationComponent(int x = 0, int y = 0)
    {
        _componentDataPointer = gnLocationComponentNew();
        X = x;
        Y = y;
        // _ecsComponentDataPtr = ECSContext.geContextComponentNew(context, 1, ComponentPtr);
        _ecsComponentDataPtr = ECS.NewComponent(1, ComponentPtr );
    }
    public int X
    {
        get => gnLocationComponentGetX(ComponentPtr);
        set => gnLocationComponentSetX(ComponentPtr, value);
    }
    public int Y
    {
        get => gnLocationComponentGetY(ComponentPtr);
        set => gnLocationComponentSetY(ComponentPtr, value);
    }

    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int gnLocationComponentGetX(IntPtr component);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int gnLocationComponentGetY(IntPtr component);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetX(IntPtr component, int x);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetY(IntPtr component, int y);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetXY(IntPtr component, int x, int y);
    [DllImport("../build/lib/libSupergoonEngine")]
    protected static extern IntPtr gnLocationComponentNew();
}