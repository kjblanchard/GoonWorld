using GoonEngine.Components;
using GoonEngine.Models;
namespace GoonEngine.Objects;

public abstract class ObjectBase<T> : GameObject, IAnimate where T: GameObject
{

    protected static Animator<T> _animator = new();
    protected PhysicsComponent _physicsComponent;
    protected AnimationComponent<T> _animationComponent;

    public ObjectBase(object data): base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)){GravityEnabled = true};
        _animationComponent = new AnimationComponent<T>(_animator);
        AddComponent(_physicsComponent, _animationComponent);
    }


}