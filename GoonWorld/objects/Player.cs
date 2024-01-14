namespace GoonEngine.Objects;
using GoonEngine.Components;

class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;

    Point size;

    public Player(object data) : base()
    {
        if (!(data is TiledCS.TiledObject castedData))
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        size.X = (int)castedData.width;
        size.Y = (int)castedData.height;
        // _keyboardComponent = new KeyboardComponent();
        // _drawComponent = new DrawComponent(_locationComponent);
        // _drawComponent.Data.Width = 44;
        // _drawComponent.Data.Height = 50;
        // SetControllerButtons();
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height));
        _physicsComponent.Body.GravityEnabled = 1;
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.ButtonMap[Models.EngineButtons.A] = Models.SDL_Scancode.SDL_SCANCODE_SPACE;
        Debug.Level = Debug.LogLevel.Error;
        UpdateGameObjects.Add(this);
        DrawGameObjects.Add(this);
    }

    private void SetControllerButtons()
    {
        // _keyboardComponent.SetControllerButton(Models.EngineButtons.ebA, Models.SDL_Scancode.SDL_SCANCODE_SPACE);
        // _keyboardComponent.SetControllerButton(Models.EngineButtons.ebLeft, Models.SDL_Scancode.SDL_SCANCODE_A);
        // _keyboardComponent.SetControllerButton(Models.EngineButtons.ebRight, Models.SDL_Scancode.SDL_SCANCODE_D);
        // _keyboardComponent.SetControllerButton(Models.EngineButtons.ebUp, Models.SDL_Scancode.SDL_SCANCODE_W);
        // _keyboardComponent.SetControllerButton(Models.EngineButtons.ebDown, Models.SDL_Scancode.SDL_SCANCODE_S);
    }

    // const int initialMoveSpeed = 20;
    // // private float LeftRightMovement => _physicsComponent.Bod.Velocity.x == 0 ? initialMoveSpeed : initialMoveSpeed * 200 * DeltaTime.Seconds;
    // private float LeftRightMovement => _physicsComponent.Body.Velocity.X == 0 ? initialMoveSpeed : 5;


    private void HandleInput()
    {
        // if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebLeft))
        // {
        //     var movementBase = LeftRightMovement;
        //     _physicsComponent.Body.Velocity.X += -movementBase;
        //     // Location.x -= 1;
        // }
        // if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebRight))
        // {
        //     var movementBase = LeftRightMovement;
        //     // Location.x += 1;
        //     _physicsComponent.Body.Velocity.X += movementBase;
        // }
        // if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebUp))
        // {
        //     // Location.y -= 1;
        //     _physicsComponent.Body.Velocity.Y -= 5;
        // }
        // if (_keyboardComponent.GetButtonDown(Models.EngineButtons.ebDown))
        // {
        //     // Location.y += 1;
        //     _physicsComponent.Body.Velocity.Y += 5;
        // }

        if(_keyboardComponent.IsButtonPressed(Models.EngineButtons.A))
        {
            Debug.InfoMessage("Just pressed the button!");
        }

    }

    public override void Update()
    {
        base.Update();
        HandleInput();
    }
    public void DrawMan()
    {
        var rect = new Rect(Location.X, Location.Y, size.X, size.Y);
        Api.Rendering.DrawDebugRect(ref rect);
    }

}