using GoonEngine.Content;
using GoonEngine.Interfaces;

namespace GoonEngine.Components;

public class SpriteComponent : Component, IDraw

{
    private Image? _loadedImage;
    public Rect SpriteImageRect;
    public bool Mirror { get; set; } = false;
    public Point Size;
    public Point Offset;
    public Rect DrawImageRect => new Rect(Parent.Location.X + Offset.X, Parent.Location.Y + Offset.Y, Size.X, Size.Y);
    public bool Visible { get; set; } = true;

    public SpriteComponent(Rect spriteRect, string filepath = null)
    {
        SpriteImageRect = spriteRect;
        Size.X = spriteRect.Width;
        Size.Y = spriteRect.Height;
        if (filepath != null)
            _loadedImage = Image.LoadImage(filepath);
    }

    public void Draw()
    {
        if (!Enabled || !Visible || _loadedImage == null)
            return;
        var drawRect = DrawImageRect;
        Api.Rendering.DrawTexture(_loadedImage.LoadedImagePtr, ref SpriteImageRect, ref drawRect);
    }
    public override void OnComponentAdd(GameObject parent)
    {
        base.OnComponentAdd(parent);
        GameObject.CurrentDrawComponents.Add(this);
    }

}