using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct BoundingBox
{
    public double x, y, w, h;
}