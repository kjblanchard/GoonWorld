namespace GoonEngine;
public abstract class Component
{
    public IntPtr ComponentPtr => _componentDataPointer;
    protected IntPtr _componentDataPointer;
    public IntPtr ECSComponentPtr => _ecsComponentDataPtr;
    protected IntPtr _ecsComponentDataPtr;

}