
using System.Runtime.InteropServices;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential)]
public struct Color
{
    public Color(int r, int g, int b, int a)
    {
        R = (byte)r;
        G = (byte)g;
        B = (byte)b;
        A = (byte)a;

    }
    public byte R;
    public byte G;
    public byte B;
    public byte A;
}