using System.Runtime.InteropServices;
using GoonEngine.Models;
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
    public Rect(BoundingBox box)
    {
        X = (int)box.X;
        Y = (int)box.Y;
        Width = (int)box.Width;
        Height = (int)box.Height;
    }
    public int X;
    public int Y;
    public int Width;
    public int Height;
}