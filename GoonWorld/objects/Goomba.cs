using GoonEngine.Models;

namespace GoonEngine.Objects;

public class Goomba : ObjectBase<Goomba>
{
    private bool _movingRight = false;
    private const float _moveSpeed = 40;
    private const float _showDeadTime = 0.20f;
    private float _currentDeadTime = 0;

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
                _animationComponent.Mirror = false;
                _movingRight = true;
                break;
            case (int)OverlapDirections.gpOverlapRight:
                _animationComponent.Mirror = true;
                _movingRight = false;
                break;
        }
    }

    public static void CreateAnimations()
    {
        _animator.LoadAnimationFile("goomba");
        _animator.AddAnimationTransition("walk", "dead", WalkToDeadAnim);
    }

    public static bool WalkToDeadAnim(Goomba goomba) => goomba._isDead;

    public override void Update()
    {
        base.Update();
        if (!_isDead)
        {
            _physicsComponent.Velocity.X = _movingRight ? _moveSpeed : -_moveSpeed;
        }
        else
        {
            _currentDeadTime += (float)DeltaTime.TotalSeconds;
            if (_currentDeadTime >= _showDeadTime)
            {
                Enabled = false;
                // _enabled = false;

            }
        }
    }
    public override void TakeDamage()
    {
        _physicsComponent.Velocity.X = 0;
        _isDead = true;
    }
}