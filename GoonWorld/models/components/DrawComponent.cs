using System.Runtime.InteropServices;
namespace GoonEngine.Models;
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct DrawComponent
{
    public bool drawDebug;
    public int sizeX;
    public int sizeY;
}