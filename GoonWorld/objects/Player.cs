namespace GoonEngine.Objects;
using GoonEngine.Components;
using GoonEngine.Models;

public class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    private DrawComponent _drawComponent;
    protected PhysicsComponent _physicsComponent;
    public static Api.Physics.Body.BodyOverlapDelegate PlayerGoombaOverlapFunc;

    public Player(object data) : base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) { BodyType = 2, GravityEnabled = true };
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.LoadControllerSettingsFromConfig(0);
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        AddComponent(_drawComponent, _physicsComponent, _keyboardComponent);
        PlayerGoombaOverlapFunc = PlayerGoombaOverlap;
        var loadedImage = Content.Image.LoadImage("mario_and_items");
        Api.Physics.Body.gpBodyAddOverlapBeginFunc(2, 3, PlayerGoombaOverlapFunc);

    }
    public static void PlayerGoombaOverlap(ref Models.Body playerBody, ref Models.Body goombaBody, ref Overlap overlap)
    {
        Player player = (Player)PhysicsComponent.GetGameObjectWithPhysicsBodyNum(playerBody.bodyNum);
        Goomba goomba = (Goomba)PhysicsComponent.GetGameObjectWithPhysicsBodyNum(goombaBody.bodyNum);
        if (player == null || goomba == null)
            Debug.InfoMessage("Borked");
        if (overlap.OverlapDirection == (int)OverlapDirections.gpOverlapDown)
        {

            playerBody.Velocity.Y -= 500;
        }
        Debug.InfoMessage("Wow it works");

    }



    private void HandleInput()
    {
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadUp))
        {
            _physicsComponent.Velocity.Y -= 5;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadDown))
        {
            _physicsComponent.Velocity.Y += 5;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadLeft))
        {
            _physicsComponent.Velocity.X -= 15;

        }
        if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.DPadRight))
        {
            _physicsComponent.Velocity.X += 15;

        }
        if (_keyboardComponent.IsButtonPressed(SdlGameControllerButton.A))
        {
            Debug.InfoMessage("Just pressed the button!");
        }
        else if (_keyboardComponent.IsButtonDown(SdlGameControllerButton.A))
        {
            Debug.InfoMessage("Holding the button!");
        }
        else if (_keyboardComponent.IsButtonReleased(SdlGameControllerButton.A))
        {
            Debug.InfoMessage("Just released the button!");
        }
        if (_keyboardComponent.IsButtonPressed(SdlGameControllerButton.LeftShoulder))
        {
            Environment.Exit(0);
        }

    }

    public override void Update()
    {
        base.Update();
        HandleInput();
    }
}