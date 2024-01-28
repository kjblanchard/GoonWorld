using GoonEngine.Models;
namespace GoonEngine.Components;
public class PhysicsComponent : Component
{
    public static T GetGameObjectWithPhysicsBodyNum<T>(int bodyNum) where T : GameObject => _bodyNumToGameObjectDictionary.TryGetValue(bodyNum, out var gameobj) ? (T)gameobj.Parent : null;
    private static List<PhysicsComponent> _physicsComponents = new();

    private static Dictionary<int, PhysicsComponent> _bodyNumToGameObjectDictionary = new();
    public ref int BodyType => ref Body.BodyType;
    private List<Api.Physics.Body.BodyOverlapDelegate> Delegates = new();
    public ref Vector2 Acceleration => ref Body.Acceleration;
    public ref Vector2 Velocity => ref Body.Velocity;
    public bool IsOnGround => Api.Physics.Body.gpBodyIsOnGround(ref Body);
    public ref BoundingBox BoundingBox => ref Body.BoundingBox;
    public bool StaticCollisionEnabled
    {
        get => Body.StaticCollisionEnabled == 0 ? false : true;
        set => Body.StaticCollisionEnabled = value ? 1 : 0;
    }
    public bool GravityEnabled
    {
        get => Body.GravityEnabled == 0 ? false : true;
        set => Body.GravityEnabled = value ? 1 : 0;
    }
    public HashSet<int> lastFrameOverlaps = new();
    private unsafe ref Body Body => ref *(Body*)_bodyPtr;
    private IntPtr _bodyPtr = IntPtr.Zero;


    public PhysicsComponent(BoundingBox bodyRectangle)
    {
        _bodyPtr = Api.Physics.Body.gpBodyNew(bodyRectangle);
        var bodyNum = Api.Physics.Scene.gpSceneAddBody(_bodyPtr);
        Body.GravityEnabled = 0;
        _bodyNumToGameObjectDictionary[bodyNum] = this;
        _physicsComponents.Add(this);
    }

    public delegate void OverlapDelegate<T>(T overlapType, ref Overlap overlap);
    public delegate void StaticOverlapDelegate(ref Body body, ref Overlap overlap);
    public void AddStaticBodyOverlapBeginFunc(StaticOverlapDelegate func)
    {
        Api.Physics.Body.BodyOverlapDelegate function = (ref Body body, ref Body overlapBody, ref Overlap overlap) =>
        {
            if (body.BodyNum != Body.BodyNum)
                return;
            func(ref overlapBody, ref overlap);
        };
        Delegates.Add(function);
        Api.Physics.Body.gpBodyAddOverlapBeginFunc(BodyType, (int)BodyTypes.Static, function);
    }
    //TODO we need a way to be able to remove these delegates..?
    public void AddOverlapBeginFunc<T>(int targetBodyType, OverlapDelegate<T> func) where T : GameObject
    {
        Api.Physics.Body.BodyOverlapDelegate function = (ref Body body, ref Body overlapBody, ref Overlap overlap) =>
        {

            if (body.BodyNum != Body.BodyNum)
                return;
            var overlapInstance = GetGameObjectWithPhysicsBodyNum<T>(overlapBody.BodyNum);
            if (overlapInstance == null)
                return;
            func(overlapInstance, ref overlap);
        };
        Delegates.Add(function);
        Api.Physics.Body.gpBodyAddOverlapBeginFunc(BodyType, targetBodyType, function);
    }

    public unsafe static void PhysicsUpdate()
    {
        _physicsComponents.ForEach(component =>
        {
            if (component.Parent == null)
                return;
            // if (component.BodyType == (int)BodyTypes.DeathBox)
            //     return;
            component.Parent.Location.X = (int)component.BoundingBox.X;
            component.Parent.Location.Y = (int)component.BoundingBox.Y;
            component.lastFrameOverlaps.Clear();
            for (int i = 0; i < component.Body.NumOverlappingBodies; i++)
            {
                Overlap* overlapPtr = (Overlap*)IntPtr.Add(component.Body.Overlaps, sizeof(Overlap) * i);
            }
        });

    }
}