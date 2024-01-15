using GoonEngine.Models;
using Microsoft.VisualBasic;
namespace GoonEngine.Components;
public class PhysicsComponent : Component
{
    public static GameObject? GetGameObjectWithPhysicsBodyNum(int bodyNum) => _bodyNumToGameObjectDictionary.TryGetValue(bodyNum, out var gameobj) ? gameobj.Parent : null;
    private static List<PhysicsComponent> _physicsComponents = new();

    private static Dictionary<int, PhysicsComponent> _bodyNumToGameObjectDictionary = new();
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
        _bodyNumToGameObjectDictionary[bodyNum] = this;
        _physicsComponents.Add(this);
    }

    public static void PhysicsUpdate()
    {
        _physicsComponents.ForEach(component =>
        {
            if (component.Parent == null)
                return;
            component.Parent.Location.X = (int)component.BoundingBox.X;
            component.Parent.Location.Y = (int)component.BoundingBox.Y;
        });

    }
}