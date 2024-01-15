namespace GoonEngine.Components;

public class DrawComponent : Component
{
    public ref Point Size => ref _size;
    public bool Enabled { get; set; } = true;
    private Point _size;
    public DrawComponent() { }
    public DrawComponent(int x, int y)
    {
        _size.X = x;
        _size.Y = y;
    }
    public DrawComponent(Point size)
    {
        _size.X = size.X;
        _size.Y = size.Y;
    }
    public void Draw()
    {
        if (!Enabled)
            return;
        var rect = new Rect(Parent.Location.X, Parent.Location.Y, _size.X, _size.Y);
        Api.Rendering.DrawDebugRect(ref rect);
    }
    public override void OnComponentAdd(GameObject parent)
    {
        base.OnComponentAdd(parent);
        GameObject.DrawGameObjects.Add(this);
    }
}
