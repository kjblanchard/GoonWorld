using GoonEngine.Components;

namespace GoonEngine.Objects;

public class Brick : GameObject
{
    PhysicsComponent physicsComponent;
    SpriteComponent spriteComponent;
    public Brick(object data) : base()
    {
        if (data is not TiledCS.TiledObject castedData)
            throw new Exception("Loading a player with no data somehow!");
        Location.X = (int)castedData.x;
        Location.Y = (int)castedData.y;
        physicsComponent = new PhysicsComponent(new Models.BoundingBox(castedData.x, castedData.y, castedData.width, castedData.height)) {GravityEnabled = false};
        spriteComponent = new SpriteComponent(new Rect(16, 0, 16, 16), "blocks.png");
        AddComponent(physicsComponent, spriteComponent);
    }

}