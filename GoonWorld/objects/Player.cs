namespace GoonEngine.Objects;
class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    private PhysicsComponent _physicsComponent;
    public Player(object data) : base()
    {
        var castedData = (TiledCS.TiledObject)data;
        if (castedData == null)
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _keyboardComponent = new KeyboardComponent();
        _drawComponent = new DrawComponent(_locationComponent) { Width = (int)castedData.width, Height = (int)castedData.height };
        _drawComponent.Width = 44;
        _drawComponent.Height = 50;
        SetControllerButtons();
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height));
        AddComponent(_keyboardComponent, _drawComponent);
        Debug.Level = Debug.LogLevel.Error;
    }

    private void SetControllerButtons()
    {
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebA, Models.SDL_Scancode.SDL_SCANCODE_SPACE);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebLeft, Models.SDL_Scancode.SDL_SCANCODE_A);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebRight, Models.SDL_Scancode.SDL_SCANCODE_D);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebUp, Models.SDL_Scancode.SDL_SCANCODE_W);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebDown, Models.SDL_Scancode.SDL_SCANCODE_S);
    }

    const int initialMoveSpeed = 20;
    // private float LeftRightMovement => _physicsComponent.Bod.Velocity.x == 0 ? initialMoveSpeed : initialMoveSpeed * 200 * DeltaTime.Seconds;
    private float LeftRightMovement => _physicsComponent.Bod.Velocity.x == 0 ? initialMoveSpeed : 5;

    private void HandleInput()
    {
        if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebLeft))
        {
            var movementBase = LeftRightMovement;
            _physicsComponent.Bod.Velocity.x += -movementBase;
        }
        if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebRight))
        {
            var movementBase = LeftRightMovement;
            _physicsComponent.Bod.Velocity.x += movementBase;
        }
        if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebUp))
        {
            _physicsComponent.Bod.Velocity.y -= 5;
        }
        if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebDown))
        {
            _physicsComponent.Bod.Velocity.y += 5;
        }

    }

    public override void Update()
    {
        base.Update();
        Location.X = (int)Math.Round(_physicsComponent.Bod.BoundingBox.x);
        Location.Y = (int)Math.Round(_physicsComponent.Bod.BoundingBox.y);
        // Debug.Log.Info($"Velocity is X: {_physicsComponent.Bod.Velocity.x} Y: {_physicsComponent.Bod.Velocity.y}");
        HandleInput();
    }

}