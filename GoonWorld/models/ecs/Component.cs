using System.Runtime.InteropServices;
namespace GoonEngine.Models;
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Component
{
    public IntPtr Parent;
    public uint Type;
    public IntPtr Data;
    public IntPtr NextComponent;
}