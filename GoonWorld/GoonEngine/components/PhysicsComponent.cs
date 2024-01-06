using GoonEngine.Models;
namespace GoonEngine;
public class PhysicsComponent
{
    public unsafe ref Body Body => ref *(Body*)_bodyPtr;
    private IntPtr _bodyPtr = IntPtr.Zero;

    public PhysicsComponent(BoundingBox bodyRectangle)
    {
        _bodyPtr = Api.Physics.Body.gpBodyNew(bodyRectangle);
        var bodyNum = Api.Physics.Scene.gpSceneAddBody(_bodyPtr);

    }
}