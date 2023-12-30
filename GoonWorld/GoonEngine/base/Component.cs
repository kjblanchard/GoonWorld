namespace GoonEngine;
public abstract class Component
{
    public IntPtr ComponentPtr => _componentDataPointer;
    protected IntPtr _componentDataPointer;
    public IntPtr ECSComponentPtr => _ecsComponentDataPtr;
    protected IntPtr _ecsComponentDataPtr;

}
public static class ComponentTypes
{
    public const int TAG_COMPONENT = 0;
    public const int LOCATION_COMPONENT = 1;
    public const int SCRIPT_COMPONENT = 2;
}