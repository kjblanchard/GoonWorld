using System.Runtime.InteropServices;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Point
{
    public int X, Y;
}