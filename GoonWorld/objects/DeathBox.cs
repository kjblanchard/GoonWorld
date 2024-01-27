namespace GoonEngine.Objects;
using GoonEngine.Components;

public class DeathBox : GameObject
{
    protected PhysicsComponent _physicsComponent;
    protected DrawComponent _drawComponent;

    public DeathBox(object data) : base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) { BodyType = (int)BodyTypes.DeathBox, GravityEnabled = false };
        // _physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) { BodyType = (int)BodyTypes.DeathBox};
        // _physicsComponent.GravityEnabled = false;
        _drawComponent = new DrawComponent((int)castedData.width, (int)castedData.height) { Color = new Color(255, 0, 0, 255)};
        AddComponent(_physicsComponent);
        AddComponent(_drawComponent);
    }

    public override void Update()
    {
        base.Update();
    }
}