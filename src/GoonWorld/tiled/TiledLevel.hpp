#pragma once
#include <memory>
#include <GoonWorld/tiled/TiledMap.hpp>
struct SDL_Surface;
struct SDL_Texture;
namespace GoonWorld
{
    /**
     * @brief A level that is loaded from tiled, this is loaded from json and contains all tiles, objects, properties, etc
     */
    class TiledLevel
    {
    public:
        /**
         * @brief Construct a new Tiled Level object
         *
         * @param filename The file to load from json
         */
        TiledLevel(const char *filename);
        std::vector<TiledMap::TiledObject> GetAllObjects();
        inline std::vector<TiledMap::TiledObject> GetAllSolidObjects() const { return _mapData->SolidObjects; }
        ~TiledLevel();
        /**
         * @brief Set the Texture Atlas object in the engine, so that we draw it on updates.
         */
        void SetTextureAtlas();

    private:
        /**
         * @brief Loads all the surfaces from the tilesets in this level, so that we can blit them properly
         */
        void LoadSurfaces();
        void LoadSolidObjects();
        /**
         * @brief Create a Background Atlas object and blits everything to it
         */
        void CreateBackgroundAtlas();
        /**
         * @brief Get the Surface For Gid object
         *
         * @param gid gid of the tile
         * @param tileset the tileset for this tile
         * @return struct SDL_Surface* The loaded surface
         */
        struct SDL_Surface *GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset);

    private:
        std::vector<std::pair<std::string, struct SDL_Surface *>> _loadedTilesets;
        struct SDL_Texture *_loadedAtlas;
        std::unique_ptr<TiledMap> _mapData;
    };
}