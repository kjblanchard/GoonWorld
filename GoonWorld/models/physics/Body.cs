// typedef struct gpBody
// {
//     int bodyNum;
//     int bodyType;
//     gpBB boundingBox;
//     gpVec velocity;
//     int numOverlappingBodies;
//     int gravityEnabled;
//     // int bodyOnGround;
//     struct gpOverlap** overlaps;


// } gpBody;

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Body
{
    public int bodyNum, bodyType;
    public BoundingBox BoundingBox;
    public Vector2 Velocity;
    public int NumOverlappingBodies, GravityEnabled;
    public IntPtr Overlaps;

    unsafe Overlap* GetOverlapNum(int overlapNum)
    {
        if (overlapNum < 0 || overlapNum > NumOverlappingBodies)
            return null;
        return (Overlap*)Marshal.ReadIntPtr(Overlaps, overlapNum * IntPtr.Size);

    }
}