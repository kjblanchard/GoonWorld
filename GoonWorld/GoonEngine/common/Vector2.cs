using System.Runtime.InteropServices;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Vector2
{
    public float X, Y;
}