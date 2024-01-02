
namespace GoonEngine.Objects;
class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    public Player(object data) : base()
    {
        var castedData = (TiledCS.TiledObject)data;
        if (castedData == null)
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _keyboardComponent = new KeyboardComponent();
        // _drawComponent = new DrawComponent(_locationComponent) { Width = (int)castedData.width, Height = (int)castedData.height };
        _drawComponent = new DrawComponent(_locationComponent);
        _drawComponent.Width = (int)castedData.width;
        _drawComponent.Height = (int)castedData.height;
        _drawComponent.Width = 44;
        _drawComponent.Height = 50;
        SetControllerButtons();
        AddComponent(_keyboardComponent, _drawComponent);
    }

    private void SetControllerButtons()
    {
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebA, Models.SDL_Scancode.SDL_SCANCODE_SPACE);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebLeft, Models.SDL_Scancode.SDL_SCANCODE_A);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebRight, Models.SDL_Scancode.SDL_SCANCODE_D);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebUp, Models.SDL_Scancode.SDL_SCANCODE_W);
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebDown, Models.SDL_Scancode.SDL_SCANCODE_S);
    }

    private void HandleInput()
    {
        if(_keyboardComponent.GetButtonDown(Models.EngineButtons.ebLeft))
        {
            Location.X -= 1;
        }
        if(_keyboardComponent.GetButtonDown(Models.EngineButtons.ebRight))
        {
            Location.X += 1;
        }
        if(_keyboardComponent.GetButtonDown(Models.EngineButtons.ebUp))
        {
            Location.Y -= 1;
        }
        if(_keyboardComponent.GetButtonDown(Models.EngineButtons.ebDown))
        {
            Location.Y += 1;
        }

    }

    public override void Update()
    {
        base.Update();
        HandleInput();
    }

}