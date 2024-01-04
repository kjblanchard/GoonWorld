// struct gpBody;
// typedef enum gpOverlapDirections
// {
//     gpOverlapNoOverlap = 0,
//     gpOverlapUp,
//     gpOverlapRight,
//     gpOverlapDown,
//     gpOverlapLeft
// } gpOverlapDirections;

// typedef struct gpOverlap
// {
//     int overlapDirection;
//     struct gpBody *overlapBody;
// } gpOverlap;

using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Overlap
{
    public int OverlapDirection;
    public IntPtr OverlapBody;
}