// typedef struct gpVec
// {
//     float x, y;
// } gpVec;

using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Vector2
{
    public float x, y;
}