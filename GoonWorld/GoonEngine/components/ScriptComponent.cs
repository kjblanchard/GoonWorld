using System.Runtime.InteropServices;
namespace GoonEngine;

public class ScriptComponent : Component
{
    // [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    // public delegate void UpdateDelegate(IntPtr data);
    // public UpdateDelegate UpdateFunc;
    public Api.Components.ScriptComponent.ScriptComponentDelegate UpdateFunc;
    public ScriptComponent(Api.Components.ScriptComponent.ScriptComponentDelegate updateFunc)
    {
        _componentDataPointer = Api.Components.ScriptComponent.gnScriptComponentNew();
        UpdateFunc = updateFunc;
        Api.Components.ScriptComponent.gnScriptComponentSetUpdateFunc(_componentDataPointer, UpdateFunc);
        _ecsComponentDataPtr = ECS.NewComponent(ComponentTypes.SCRIPT_COMPONENT, ComponentPtr );
    }

}