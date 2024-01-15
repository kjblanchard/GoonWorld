using System.Runtime.InteropServices;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Point
{
    public int X, Y;
    public Point(int x, int y)
    {
        X = x;
        Y = y;
    }
    public Point(float x, float y)
    {
        X = (int)x;
        Y = (int)y;
    }
}
