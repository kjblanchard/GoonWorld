#pragma once
namespace GoonWorld
{
    // struct SDL_Surface;
    class Tiled
    {

    public:
        Tiled(const char *filename);
        uint Width, Height, TileWidth, TileHeight;
    private:
    };

}