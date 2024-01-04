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

using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Body
{
    public int bodyNum, bodyType;
    public BoundingBox BoundingBox;
    // gpvec Velocity
    public int NumOverlappingBodies, GravityEnabled;
    IntPtr[] Overlaps;
}