#include <fstream>
#include <string>

#include <json/json.hpp>
#include <GoonWorld/core/Tiled.hpp>
#include <GoonEngine/SdlSurface.h>
using json = nlohmann::json;
using jsonClass = nlohmann::json_abi_v3_11_3::json;
using namespace GoonWorld;

const char *_assetPrefix = "assets/";
const std::string _tiledPrefix = "tiled/";

jsonClass GetTilesetJsonForGid(uint gid, jsonClass tilesets)
{
    jsonClass *foundTilesetJson = nullptr;
    bool found = false;
    for (auto &tileset : tilesets)
    {
        int firstGid = tileset["firstgid"];
        if (gid >= firstGid)
        {
            foundTilesetJson = &tileset;
        }
        break;
    }
    if (!foundTilesetJson)
    {
        printf("Could not find tileset for git %ud\n", gid);
        return nullptr;
    }
    std::string tilesetSource = (*foundTilesetJson)["source"];
    std::ifstream tilesetRead(_assetPrefix + _tiledPrefix + tilesetSource);
    json tilesetData = json::parse(tilesetRead);
    // do the thing
    return *foundTilesetJson;
}

Tiled::Tiled(const char *filename)
{

    auto pathPrefix = _assetPrefix + _tiledPrefix + filename + ".tmj";
    std::ifstream file(pathPrefix);
    json data = json::parse(file);
    auto tilesets = data["tilesets"];

    // auto loadedTilesets = LoadTilesets(tilesets);
    Width = data["width"];
    Height = data["height"];
    TileWidth = data["tilewidth"];
    TileHeight = data["tileheight"];
    for (auto &layer : data["layers"])
    {
        std::string layerName = layer["name"];
        if (layerName == "background")
        {
            for (auto &groupLayer : layer["layers"])
            {
                std::string backgroundLayerName = groupLayer["name"];
                uint height = groupLayer["height"];
                uint width = groupLayer["width"];
                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        auto index = (y * width) + x;             // Assuming the default render order is used which is from right to bottom
                        auto tileGid = groupLayer["data"][index]; // The tileset tile index
                        // GetTIlesetByGid(1, tilesets);
                        if (tileGid == 0)
                            continue;
                        printf("Got here\n");
                        // var tilesetMap = LoadedMap.GetTiledMapTileset(tileGid);
                        // var tileset = tilesets[tilesetMap.firstgid];
                        // var tiledTile = LoadedMap.GetTiledTile(tilesetMap, tileset, tileGid);
                        // IntPtr loadedTileset = IntPtr.Zero;
                        // var dstX = x * LoadedMap.TileWidth;
                        // var dstY = y * LoadedMap.TileHeight;
                        // if (tiledTile == null)
                        // {
                        //     // this is a tile, use regular x for destination
                        //     loadedTileset = GetImageFromFilepath(tileset.Image.source);
                        // }
                        // else
                        // {
                        //     // This is an image tile.
                        //     loadedTileset = GetImageFromFilepath(tiledTile.image.source);
                        //     dstY -= LoadedMap.TileHeight;
                        // }
                        // var srcRect = new Rect(LoadedMap.GetSourceRect(tilesetMap, tileset, tileGid));
                        // var dstRect = new Rect(
                        //     dstX,
                        //     dstY,
                        //     srcRect.Width,
                        //     srcRect.Height);
                        // BlitSurface(loadedTileset, ref srcRect, atlas, ref dstRect);
                    }
                }
            }
        }
    }
}