using System.Runtime.InteropServices;
namespace GoonEngine;
public class TagComponent : Component
{
    // public TagComponent(IntPtr context, params string[] tags)
    public TagComponent(params string[] tags)
    {
        _componentDataPointer = gnTagComponentNew();
        // _ecsComponentDataPtr = ECSContext.geContextComponentNew(context, 0, ComponentPtr);
        _ecsComponentDataPtr = ECS.NewComponent(0, ComponentPtr);
        foreach (var tag in tags)
        {
            AddTag(tag);
        }
    }
    public bool HasTag(string tag) => gnTagComponentHasTag(ComponentPtr, tag);
    public bool AddTag(string tag) => gnTagComponentAddTag(ComponentPtr, tag);
    [DllImport("../build/lib/libSupergoonEngine")]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool gnTagComponentAddTag(IntPtr component, string tag);
    [DllImport("../build/lib/libSupergoonEngine")]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool gnTagComponentHasTag(IntPtr component, string tag);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr gnTagComponentNew();
}