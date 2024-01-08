using System.Runtime.InteropServices;
using TiledCS;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential)]
public struct Rect
{
    public Rect(int x, int y, int w, int h)
    {
        X = x;
        Y = y;
        Width = w;
        Height = h;

    }
    public Rect(TiledSourceRect rect)
    {
        X = rect.x;
        Y = rect.y;
        Width = rect.width;
        Height = rect.height;
    }
    public int X;
    public int Y;
    public int Width;
    public int Height;
}