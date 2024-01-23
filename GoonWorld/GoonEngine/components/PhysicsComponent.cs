using GoonEngine.Models;
namespace GoonEngine.Components;
public class PhysicsComponent : Component
{
    public static GameObject? GetGameObjectWithPhysicsBodyNum(int bodyNum) => _bodyNumToGameObjectDictionary.TryGetValue(bodyNum, out var gameobj) ? gameobj.Parent : null;
    private static List<PhysicsComponent> _physicsComponents = new();

    private static Dictionary<int, PhysicsComponent> _bodyNumToGameObjectDictionary = new();
    public ref int BodyType => ref Body.bodyType;
    private List<Api.Physics.Body.BodyOverlapDelegate>Delegates = new();
    public ref Vector2 Velocity => ref Body.Velocity;
    public ref BoundingBox BoundingBox => ref Body.BoundingBox;
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
        _bodyNumToGameObjectDictionary[bodyNum] = this;
        _physicsComponents.Add(this);
    }

    // public void AddOverlapFunc(int targetBodyType, Api.Physics.Body.BodyOverlapDelegate func)
    public void AddOverlapBeginFunc(int targetBodyType, Api.Physics.Body.BodyOverlapDelegate func)
    {
        Delegates.Add(func);
        Api.Physics.Body.gpBodyAddOverlapBeginFunc(BodyType,targetBodyType, func);
    }

    public unsafe static void PhysicsUpdate()
    {
        _physicsComponents.ForEach(component =>
        {
            if (component.Parent == null)
                return;
            component.Parent.Location.X = (int)component.BoundingBox.X;
            component.Parent.Location.Y = (int)component.BoundingBox.Y;
            component.lastFrameOverlaps.Clear();
            for (int i = 0; i < component.Body.NumOverlappingBodies; i++)
            {
                // Overlap valueAtIndexi = *(Overlap*)Marshal.ReadInt32(component.Body.Overlaps + sizeof(Overlap) * i);

                Overlap* overlapPtr = (Overlap*)IntPtr.Add(component.Body.Overlaps, sizeof(Overlap) * i);
                // Debug.InfoMessage($"I'm overlapping with body num {overlapPtr->OverlapBody}");
                // component.lastFrameOverlaps.Add(component.Body.Overlaps[i]);
            }
        });

    }
}