#pragma once
#include <memory>
#include <GoonWorld/tiled/TiledMap.hpp>
struct SDL_Surface;
struct SDL_Texture;
namespace GoonWorld
{
    class Tiled
    {

    public:
        Tiled(const char *filename);
        ~Tiled();
        void SetTextureAtlas();

    private:
        uint Width, Height, TileWidth, TileHeight;
        std::vector<std::pair<std::string, struct SDL_Surface *>> _loadedTilesets;
        struct SDL_Surface *GetSurfaceForGid(uint gid, TiledMap::Tileset &tileset);
        void LoadSurfaces();
        struct SDL_Surface *DrawBackgroundAtlas();
        struct SDL_Texture *_loadedAtlas;
        std::shared_ptr<TiledMap> _mapData;
    };
}