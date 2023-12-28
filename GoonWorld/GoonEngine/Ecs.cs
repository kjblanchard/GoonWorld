using System.Runtime.InteropServices;
namespace GoonEngine;

public static class ECS
{
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geComponentNew(int type, IntPtr data);

    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geContextNew();
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geContextEntityNew(IntPtr context);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geGetEntityById(IntPtr context, int id);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geContextComponentNew(IntPtr context, int type, IntPtr data);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void SystemDelegate(ref IntPtr context, int type, IntPtr data);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int geContextSystemNew(IntPtr context, SystemDelegate system, int systemType);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int geEntityAddComponent(IntPtr entity, IntPtr component);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern bool geEntityHasComponent(IntPtr entity, uint componentBit);
    [return: MarshalAs(UnmanagedType.I1)]
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geEntityGetComponent(IntPtr entity, uint componentBit);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int geEntityRemoveComponent(IntPtr entity, uint componentBit);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr gnLocationComponentNew();
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr gnTagComponentNew();
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int gnTagComponentAddTag(IntPtr component, string tag);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int gnTagComponentHasTag(IntPtr component, string tag);

}

public struct TagComponent: IComponent
{
    public TagComponent(IntPtr context)
    {
        _ptr = ECS.gnTagComponentNew();
        _ptr2 = ECS.geContextComponentNew(context, 0, _ptr);
    }
    public IntPtr ComponentPtr { get => _ptr; set => _ptr = value; }
    private IntPtr _ptr;
    public IntPtr ECSComponentPtr { get => _ptr2; set => _ptr2 = value; }
    private IntPtr _ptr2;
    public bool HasTag(string tag) => gnTagComponentHasTag(_ptr, tag);
    public bool AddTag(string tag) => gnTagComponentAddTag(_ptr, tag);

    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern bool gnTagComponentAddTag(IntPtr component, string tag);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern bool gnTagComponentHasTag(IntPtr component, string tag);
}

public class LocationComponent: IComponent
{
    public LocationComponent(IntPtr context, int x = 0, int y = 0)
    {
        _ptr = ECS.gnLocationComponentNew();
        X = x;
        Y = y;
        _ptr2 = ECS.geContextComponentNew(context, 1, _ptr);
    }
    public int X
    {
        get => gnLocationComponentGetX(_ptr);
        set => gnLocationComponentSetX(_ptr, value);
    }
    public int Y
    {
        get => gnLocationComponentGetY(_ptr);
        set => gnLocationComponentSetY(_ptr, value);
    }
    public IntPtr ComponentPtr { get => _ptr; set => _ptr = value; }
    private IntPtr _ptr;
    public IntPtr ECSComponentPtr { get => _ptr2; set => _ptr2 = value; }
    private IntPtr _ptr2;

    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int gnLocationComponentGetX(IntPtr component);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int gnLocationComponentGetY(IntPtr component);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetX(IntPtr component, int x);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetY(IntPtr component, int y);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void gnLocationComponentSetXY(IntPtr component, int x, int y);
}