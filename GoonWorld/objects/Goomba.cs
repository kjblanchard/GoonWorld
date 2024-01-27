namespace GoonEngine.Objects;
using GoonEngine.Components;

public class Goomba : GameObject
{
    private DrawComponent _drawComponent;
    protected PhysicsComponent _physicsComponent;

    public Goomba(object data) : base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) { BodyType = (int)BodyTypes.Goomba, GravityEnabled = true };
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height);
        AddComponent(_drawComponent, _physicsComponent);
        _drawComponent.Color = new Color(0, 0, 255, 255);
    }

    public override void Update()
    {
        base.Update();
    }
}