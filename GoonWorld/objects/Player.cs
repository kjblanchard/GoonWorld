namespace GoonEngine.Objects;
using GoonEngine.Components;

public class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;

    public Player(object data) : base()
    {
        if (!(data is TiledCS.TiledObject castedData))
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height));
        _physicsComponent.GravityEnabled = true;
        _keyboardComponent = new KeyboardComponent();
        SetControllerButtons();
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        AddComponent(_drawComponent);
    }

    private void SetControllerButtons()
    {
        _keyboardComponent.ButtonMap[ControllerButtons.A] = SdlScancodes.SDL_SCANCODE_SPACE;
        _keyboardComponent.ButtonMap[ControllerButtons.Up] = SdlScancodes.SDL_SCANCODE_W;
        _keyboardComponent.ButtonMap[ControllerButtons.Down] = SdlScancodes.SDL_SCANCODE_S;
        _keyboardComponent.ButtonMap[ControllerButtons.Left] = SdlScancodes.SDL_SCANCODE_A;
        _keyboardComponent.ButtonMap[ControllerButtons.Right] = SdlScancodes.SDL_SCANCODE_D;
    }

    private void HandleInput()
    {
        if (_keyboardComponent.IsButtonDown(ControllerButtons.Up))
        {
            _physicsComponent.Velocity.Y -= 5;

        }
        if (_keyboardComponent.IsButtonDown(ControllerButtons.Down))
        {
            _physicsComponent.Velocity.Y += 5;

        }
        if (_keyboardComponent.IsButtonDown(ControllerButtons.Left))
        {
            _physicsComponent.Velocity.X -= 15;

        }
        if (_keyboardComponent.IsButtonDown(ControllerButtons.Right))
        {
            _physicsComponent.Velocity.X += 15;

        }
        if (_keyboardComponent.IsButtonPressed(ControllerButtons.A))
        {
            Debug.InfoMessage("Just pressed the button!");
        }

    }

    public override void Update()
    {
        base.Update();
        HandleInput();
    }
}