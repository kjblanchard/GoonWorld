namespace GoonEngine.Objects;
using GoonEngine.Components;
using GoonEngine.Models;

public class Player : ObjectBase<Player>
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    private bool _isJumping;
    private bool _canJump;
    private int _jumpVelocity = 5;
    private float _currentJumpTime;
    private float _maxJumpTime = 0.25f;

    public Player(object data) : base(data)
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.LoadControllerSettingsFromConfig(0);
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        _animationComponent.SizeMultiplier = 2;
        _physicsComponent.BodyType = (int)BodyTypes.Player;
        _physicsComponent.AddOverlapBeginFunc(3, PlayerGoombaOverlap);
        AddComponent(_keyboardComponent, _drawComponent);
    }
    public static void PlayerGoombaOverlap(ref Body playerBody, ref Body goombaBody, ref Overlap overlap)
    {
        var player = PhysicsComponent.GetGameObjectWithPhysicsBodyNum<Player>(playerBody.bodyNum);
        var goomba = PhysicsComponent.GetGameObjectWithPhysicsBodyNum<Goomba>(goombaBody.bodyNum);
        if (player == null || goomba == null)
            return;
        player.GoombaOverlapInstance(goomba, ref goombaBody, ref overlap);
    }
    public void GoombaOverlapInstance(Goomba goomba, ref Body goombaBody, ref Overlap overlap)
    {
        if (overlap.OverlapDirection == (int)OverlapDirections.gpOverlapDown)
        {
            _physicsComponent.Velocity.Y -= 500;
            _canJump = true;
        }

    }

    public static void CreateAnimations()
    {
        _animator.LoadAnimationFile("mario");
        _animator.AddAnimationTransition("idle", "walk", ShouldRun);
        _animator.AddAnimationTransition("walk", "idle", ShouldWalk);
    }

    private void HandleInput()
    {
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadLeft))
        {
            _physicsComponent.Velocity.X -= 15;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadRight))
        {
            _physicsComponent.Velocity.X += 15;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.A))
        {
            Jump();
        }
        else if (_keyboardComponent.IsButtonReleased(SdlGameControllerButton.A))
        {
            _isJumping = false;
        }
        if (_keyboardComponent.IsButtonPressed(SdlGameControllerButton.LeftShoulder))
        {
            Environment.Exit(0);
        }

    }

    public override void Update()
    {
        HandleInput();
        _canJump = _physicsComponent.IsOnGround;
        base.Update();
    }

    private void Jump()
    {
        if (_isJumping)
        {
            if (_currentJumpTime < _maxJumpTime)
            {
                Debug.InfoMessage("Jump Extension");
                _physicsComponent.Velocity.Y -= _jumpVelocity;
                _currentJumpTime += (float)DeltaTime.TotalSeconds;
            }
            else
            {
                Debug.InfoMessage("Jump maxed");
                _isJumping = _canJump = false;
            }
        }
        else if (_canJump)
        {
            Debug.InfoMessage("Big jump");
            _currentJumpTime = 0;
            _isJumping = true;
            _canJump = false;
            _physicsComponent.Velocity.Y += _jumpVelocity * 5;
        }

    }

    public static bool ShouldRun(Player player)
    {
        return player._physicsComponent.Velocity.X != 0;
    }
    public static bool ShouldWalk(Player player)
    {
        return player._physicsComponent.Velocity.X == 0;
    }
}