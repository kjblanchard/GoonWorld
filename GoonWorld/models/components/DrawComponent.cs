using System.Runtime.InteropServices;
namespace GoonEngine.Models;
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct DrawComponent
{
    public IntPtr LocationComponentPtr;
    public int ShouldDrawDebug;
    public int Width;
    public int Height;
}