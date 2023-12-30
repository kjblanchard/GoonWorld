using System.Runtime.InteropServices;

namespace GoonEngine.Models;
// Define the delegate signature

// Define the ScriptComponent struct
[StructLayout(LayoutKind.Sequential)]
public struct ScriptComponent
{
    public Api.Components.ScriptComponent.ScriptComponentDelegate UpdateFunc;
}