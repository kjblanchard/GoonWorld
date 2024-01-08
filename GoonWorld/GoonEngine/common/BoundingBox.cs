using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct BoundingBox
{
    public double X, Y, Width, Height;

    public BoundingBox(double x, double y, double w, double h)
    {
        X = x;
        Y = y;
        Width = w;
        Height = h;
    }

}