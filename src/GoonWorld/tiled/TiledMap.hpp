#pragma once
#include <string>
#include <vector>
struct SDL_Rect;

namespace GoonWorld
{
    enum class TilesetType
    {
        Default,
        Image,
        Tile,
    };

    class TiledMap
    {
    public:
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
        };
        struct LayerGroup
        {
            std::string Name;
            std::vector<Layer> Layers;
        };

    public:
        SDL_Rect GetSourceRectForGid(int gid);
        TiledMapTileset GetTiledMapTilesetForGid(int gid);
        Tileset GetTilesetForTiledMapTileset(TiledMapTileset &tilesetName);
        TiledMap(std::string filename);

    public:
        int Width, Height, TileWidth, TileHeight;
        std::vector<TiledMapTileset> TiledmapTilesets;
        std::vector<Tileset> Tilesets;
        std::vector<LayerGroup> Groups;
        std::vector<Layer> Layers;

    private:
        SDL_Rect GetSourceRectForGid_Int(int gid, TiledMap::Tileset tileset);
    };
}