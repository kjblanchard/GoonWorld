
namespace GoonEngine.Objects;
class Player : GameObject
{
    private KeyboardComponent _keyboardComponent;
    public Player(object data) : base()
    {
        var castedData = (TiledCS.TiledObject)data;
        if (castedData == null)
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _keyboardComponent = new KeyboardComponent();
        _keyboardComponent.SetControllerButton(Models.EngineButtons.ebA, Models.SDL_Scancode.SDL_SCANCODE_SPACE);
        AddComponent(_keyboardComponent);
    }

    public override void Update(IntPtr input)
    {
        base.Update(input);
        // Console.WriteLine($"I am the player, and my location is {Location.X}:  {Location.Y}!");
        if(_keyboardComponent.GetButtonPressed(Models.EngineButtons.ebA))
        {
            Console.WriteLine("Button just presed!");
        }
        if(_keyboardComponent.GetButtonHeld(Models.EngineButtons.ebA))
        {
            Console.WriteLine("Button held!");
        }
        if(_keyboardComponent.GetButtonReleased(Models.EngineButtons.ebA))
        {
            Console.WriteLine("Button released!");
        }
    }

}