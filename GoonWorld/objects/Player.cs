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
        _physicsComponent.Body.GravityEnabled = 1;
        _keyboardComponent = new KeyboardComponent();
        SetControllerButtons();
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        AddComponent(_drawComponent);
    }

    private void SetControllerButtons()
    {
        _keyboardComponent.ButtonMap[Models.EngineButtons.A] = Models.SDL_Scancode.SDL_SCANCODE_SPACE;
        _keyboardComponent.ButtonMap[Models.EngineButtons.Up] = Models.SDL_Scancode.SDL_SCANCODE_W;
        _keyboardComponent.ButtonMap[Models.EngineButtons.Down] = Models.SDL_Scancode.SDL_SCANCODE_S;
        _keyboardComponent.ButtonMap[Models.EngineButtons.Left] = Models.SDL_Scancode.SDL_SCANCODE_A;
        _keyboardComponent.ButtonMap[Models.EngineButtons.Right] = Models.SDL_Scancode.SDL_SCANCODE_D;
    }

    private void HandleInput()
    {
        if (_keyboardComponent.IsButtonDown(Models.EngineButtons.Up))
        {
            _physicsComponent.Body.Velocity.Y -= 5;

        }
        if (_keyboardComponent.IsButtonDown(Models.EngineButtons.Down))
        {
            _physicsComponent.Body.Velocity.Y += 5;

        }
        if (_keyboardComponent.IsButtonDown(Models.EngineButtons.Left))
        {
            _physicsComponent.Body.Velocity.X -= 15;

        }
        if (_keyboardComponent.IsButtonDown(Models.EngineButtons.Right))
        {
            _physicsComponent.Body.Velocity.X += 15;

        }
        if (_keyboardComponent.IsButtonPressed(Models.EngineButtons.A))
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