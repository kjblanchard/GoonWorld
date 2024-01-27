namespace GoonEngine.Objects;
using GoonEngine.Components;
using GoonEngine.Content;
using GoonEngine.Models;

public class Player : ObjectBase<Player>
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    private bool _isJumping;
    private bool _canJump;
    private bool _isDead;
    private int _jumpVelocity = 5;
    private float _currentJumpTime;
    private float _maxJumpTime = 0.25f;

    Sfx jumpSfx;

    public Player(object data) : base(data)
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        jumpSfx = Sfx.LoadSfx("jump.ogg");
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.LoadControllerSettingsFromConfig(0);
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        _animationComponent.SizeMultiplier = 2;
        _physicsComponent.BodyType = (int)BodyTypes.Player;
        AddComponent(_keyboardComponent, _drawComponent);
        _physicsComponent.AddOverlapBeginFunc<Goomba>((int)BodyTypes.Goomba, PlayerGoombaOverlap);
        _physicsComponent.AddOverlapBeginFunc<DeathBox>((int)BodyTypes.DeathBox, PlayerDeathBoxOverlap);
    }

    public void PlayerGoombaOverlap(Goomba goomba, ref Overlap overlap)
    {

        if (!_isDead && overlap.OverlapDirection == (int)OverlapDirections.gpOverlapDown)
        {
            _physicsComponent.Acceleration.Y -= 500;
            _canJump = true;
        }
        else
        {
            Die();
        }
    }

    public void PlayerDeathBoxOverlap(DeathBox deathbox, ref Overlap overlap)
    {
        Die();
    }
    private void Die()
    {
        if (_isDead)
            return;
        Api.Sound.SetPlayerLoops(0);
        Game.Global.Sound.LoadBgm("dead");
        Game.Global.Sound.PlayBgm("dead");
        _isDead = true;
        _physicsComponent.StaticCollisionEnabled = false;
        _physicsComponent.Velocity.Y = -40;
        _physicsComponent.Velocity.X = 0;

    }

    public static void CreateAnimations()
    {
        _animator.LoadAnimationFile("mario");
        _animator.AddAnimationTransition("idle", "walk", ShouldRunAnim);
        _animator.AddAnimationTransition("walk", "idle", ShouldWalkAnim);
        _animator.AddAnimationTransition("idle", "jump", ShouldJumpAnim);
        _animator.AddAnimationTransition("walk", "jump", ShouldJumpAnim);
        _animator.AddAnimationTransition("jump", "idle", ShouldIdleFromFallAnim);
        _animator.AddAnimationTransition("jump", "walk", ShouldRunFromFallAnim);
        _animator.AddAnimationTransition("walk", "turn", ShouldTurnFromWalk);
        _animator.AddAnimationTransition("turn", "walk", ShouldWalkFromTurn);
        _animator.AddAnimationTransition("walk", "dead", ShouldDieAnim);
        _animator.AddAnimationTransition("idle", "dead", ShouldDieAnim);
        _animator.AddAnimationTransition("jump", "dead", ShouldDieAnim);
    }

    private void HandleInput()
    {
        if (_isDead)
            return;
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadLeft))
        {
            _physicsComponent.Acceleration.X -= 15;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadRight))
        {
            _physicsComponent.Acceleration.X += 15;
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
        _animationComponent.Mirror = _physicsComponent.IsOnGround ? _physicsComponent.Velocity.X >= 0 ? false : true : _animationComponent.Mirror;
        base.Update();
    }

    private void Jump()
    {
        if (_isJumping)
        {
            if (_currentJumpTime < _maxJumpTime)
            {
                // Debug.InfoMessage("Jump Extension");
                _physicsComponent.Velocity.Y -= _jumpVelocity;
                _currentJumpTime += (float)DeltaTime.TotalSeconds;
            }
            else
            {
                // Debug.InfoMessage("Jump maxed");
                _isJumping = _canJump = false;
            }
        }
        else if (_canJump)
        {
            // Debug.InfoMessage("Big jump");
            Game.Global.Sound.PlaySfx(jumpSfx);
            _currentJumpTime = 0;
            _isJumping = true;
            _canJump = false;
            _physicsComponent.Velocity.Y += _jumpVelocity * 5;
        }

    }

    public static bool ShouldDieAnim(Player player)
    {
        return player._isDead;
    }

    public static bool ShouldRunAnim(Player player)
    {
        return player._physicsComponent.Velocity.X != 0;
    }
    public static bool ShouldWalkAnim(Player player)
    {
        return player._physicsComponent.Velocity.X == 0;
    }
    public static bool ShouldJumpAnim(Player player)
    {
        return player._isJumping;
    }
    public static bool ShouldTurnFromWalk(Player player)
    {
        return player._physicsComponent.Velocity.X * player._physicsComponent.Acceleration.X < 0;

    }
    public static bool ShouldWalkFromTurn(Player player)
    {
        return player._physicsComponent.Velocity.X * player._physicsComponent.Acceleration.X >= 0;

    }
    public static bool ShouldIdleFromFallAnim(Player player)
    {
        return !player._isJumping && player._physicsComponent.IsOnGround && player._physicsComponent.Velocity.X == 0;
    }

    public static bool ShouldRunFromFallAnim(Player player)
    {
        return !player._isJumping && player._physicsComponent.IsOnGround && player._physicsComponent.Velocity.X != 0;
    }
}