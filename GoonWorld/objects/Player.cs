
namespace GoonEngine.Objects;
class Player : GameObject
{
    public Player(object data) : base()
    {
        var castedData = (TiledCS.TiledObject)data;
        if (castedData == null)
            return;
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
    }

    public override void Update(IntPtr input)
    {
        base.Update(input);
        Console.WriteLine($"I am the player, and my location is {Location.X}:  {Location.Y}!");
    }

}