#pragma once
#include <memory>
#include <GoonWorld/tiled/TiledMap.hpp>
struct SDL_Surface;
struct SDL_Texture;
namespace GoonWorld
{
    class TiledLevel
    {
    public:
        TiledLevel(const char *filename);
        ~TiledLevel();
        void SetTextureAtlas();

    private:
        void LoadSurfaces();
        void CreateBackgroundAtlas();
        struct SDL_Surface *GetSurfaceForGid(int gid, const TiledMap::Tileset &tileset);

    private:
        std::vector<std::pair<std::string, struct SDL_Surface *>> _loadedTilesets;
        struct SDL_Texture *_loadedAtlas;
        std::unique_ptr<TiledMap> _mapData;
    };
}