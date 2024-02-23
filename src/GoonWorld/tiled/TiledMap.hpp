#pragma once
#include <string>
#include <vector>
struct SDL_Rect;

namespace GoonWorld
{
    /**
     * @brief The types of tilesets from tiled that is supported.
     */
    enum class TilesetType
    {
        Default,
        Image,
        Tile,
    };

    /**
     * @brief A model of a tiledmap json.
     */
    class TiledMap
    {
    public:
        struct TiledProperty
        {
            std::string Name, PropertyType;
        };
        struct TiledObject
        {
            int Id, Height, Width, X, Y;
            std::string ObjectType;
            std::vector<TiledProperty> Properties;
        };
        struct Tile
        {
            int Id, ImageHeight, ImageWidth = 0;
            std::string Image;
        };
        struct Tileset
        {
            TilesetType Type;
            std::vector<Tile> Tiles;
            int Columns, TileHeight, TileWidth, FirstGid = 0;
            std::string Name, Image;
        };

        struct TiledMapTileset
        {
            int FirstGid = 0;
            std::string Source;
        };
        struct Layer
        {
            int Height, Width;
            std::string Type, Name;
            std::vector<int> Data;
            std::vector<TiledObject> Objects;
        };
        struct LayerGroup
        {
            std::string Name;
            std::vector<Layer> Layers;
        };

    public:
        TiledMap(std::string filename);
        SDL_Rect GetGidSourceRect(int gid);
        const TiledMapTileset *const GetGidTiledMapTileset(int gid);
        const Tileset *const GetTiledMapTilesetTileset(const TiledMapTileset *tilesetName);

    public:
        int Width, Height, TileWidth, TileHeight;
        std::vector<TiledMapTileset> TiledmapTilesets;
        std::vector<Tileset> Tilesets;
        std::vector<LayerGroup> Groups;
        std::vector<Layer> Layers;
        std::vector<TiledObject> Objects;

    private:
        SDL_Rect GetSourceRectForGidWithTileset(int gid, const TiledMap::Tileset *tileset);
    };
}