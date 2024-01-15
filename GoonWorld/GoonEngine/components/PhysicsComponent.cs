using GoonEngine.Models;
namespace GoonEngine.Components;
public class PhysicsComponent : Component
{
    public ref Vector2 Velocity => ref Body.Velocity;
    public ref BoundingBox BoundingBox => ref Body.BoundingBox;
    public bool GravityEnabled
    {
        get => Body.GravityEnabled == 0 ? false : true;
        set => Body.GravityEnabled = value ? 1 : 0;

    }
    private unsafe ref Body Body => ref *(Body*)_bodyPtr;
    private IntPtr _bodyPtr = IntPtr.Zero;


    public PhysicsComponent(BoundingBox bodyRectangle)
    {
        _bodyPtr = Api.Physics.Body.gpBodyNew(bodyRectangle);
        var bodyNum = Api.Physics.Scene.gpSceneAddBody(_bodyPtr);
    }
}