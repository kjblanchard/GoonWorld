using GoonEngine.Models;

namespace GoonEngine.Objects;
// using GoonEngine.Components;

public class Goomba : ObjectBase<Goomba>
{
    private bool _movingRight = false;
    private float _moveSpeed = 40;

    public Goomba(object data) : base(data)
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _animationComponent.SizeMultiplier = 2;
        _physicsComponent.BodyType = (int)BodyTypes.Goomba;
        _physicsComponent.AddStaticBodyOverlapBeginFunc(GoombaStaticBodyOverlap);
    }

    public void GoombaStaticBodyOverlap(ref Body staticBody, ref Overlap overlap)
    {
        switch (overlap.OverlapDirection)
        {
            case (int)OverlapDirections.gpOverlapDown:
            case (int)OverlapDirections.gpOverlapUp:
                break;
            case (int)OverlapDirections.gpOverlapLeft:
                _movingRight = true;
                break;
            case (int)OverlapDirections.gpOverlapRight:
                _movingRight = false;
                break;
        }
    }

    public static void CreateAnimations()
    {
        _animator.LoadAnimationFile("goomba");
    }

    public override void Update()
    {
        base.Update();
        _physicsComponent.Velocity.X = _movingRight ? _moveSpeed : -_moveSpeed;
    }
}