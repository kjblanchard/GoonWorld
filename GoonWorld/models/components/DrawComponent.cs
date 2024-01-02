using System.Runtime.InteropServices;
namespace GoonEngine.Models;
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct DrawComponent
{
    public IntPtr locationComponent;
    public int drawDebug;
    public int sizeX;
    public int sizeY;
}