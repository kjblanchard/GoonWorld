namespace GoonEngine.Objects;
using GoonEngine.Components;

public class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    protected PhysicsComponent _physicsComponent;

    public Player(object data) : base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) { GravityEnabled = true };
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.LoadControllerSettingsFromConfig(0);
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        AddComponent(_drawComponent, _physicsComponent, _keyboardComponent);
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