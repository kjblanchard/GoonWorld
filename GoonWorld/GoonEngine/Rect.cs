using System.Runtime.InteropServices;
using TiledCS;
namespace GoonEngine;

[StructLayout(LayoutKind.Sequential)]
public struct SDL_Rect
{
    public SDL_Rect(int x, int y, int w, int h)
    {
        this.x = x;
        this.y = y;
        width = w;
        height = h;

    }
    public SDL_Rect(TiledSourceRect rect)
    {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;
    }
    public int x;
    public int y;
    public int width;
    public int height;
}