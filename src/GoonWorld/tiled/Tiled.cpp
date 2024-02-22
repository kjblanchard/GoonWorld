#include <fstream>
#include <string>
#include <json/json.hpp>
#include <GoonWorld/tiled/Tiled.hpp>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/shared/Constants.hpp>
using json = nlohmann::json;
using loadSurface = std::vector<std::pair<std::string, SDL_Surface *>>;
using namespace GoonEngine;
using namespace GoonWorld;

Tiled::Tiled(const char *filename)
{
    _mapData = std::make_shared<TiledMap>(filename);
    LoadSurfaces();
    // _loadedAtlas = DrawBackgroundAtlas();
    _loadedAtlas = CreateTextureFromSurface(DrawBackgroundAtlas());
}

Tiled::~Tiled()
{
    DestroyTexture(_loadedAtlas);
}

void Tiled::SetTextureAtlas()
{
    auto rect = SDL_Rect{
        0,
        0,
        _mapData->Width * _mapData->TileWidth,
        _mapData->Height * _mapData->TileHeight,
    };
    SetBackgroundAtlas(_loadedAtlas, &rect);
}
void Tiled::LoadSurfaces()
{
    for (auto &tileset : _mapData->Tilesets)
    {
        if (tileset.Type == TilesetType::Image)
        {
            for (auto &tile : tileset.Tiles)
            {
                auto surfacePath = AssetPrefix + TiledPrefix + tile.Image;
                auto surface = LoadSurfaceFromFile(surfacePath.c_str());
                _loadedTilesets.push_back({tile.Image, surface});
            }
        }
        else
        {
            auto surfacePath = AssetPrefix + TiledPrefix + tileset.Image;
            auto surface = LoadSurfaceFromFile(surfacePath.c_str());
            _loadedTilesets.push_back({tileset.Image, surface});
        }
    }
}
SDL_Surface *Tiled::GetSurfaceForGid(uint gid, TiledMap::Tileset &tileset)
{
    if (tileset.Type == TilesetType::Image)
    {
        for (auto &tile : tileset.Tiles)
        {
            if (tile.Id + tileset.FirstGid == gid)
            {
                for (auto surface : _loadedTilesets)
                {
                    if (surface.first == tile.Image)
                        return surface.second;
                }
            }
        }
    }
    else
    {
        for (auto surface : _loadedTilesets)
        {
            if (surface.first == tileset.Image)
                return surface.second;
        }
    }
    printf("Could not find loaded surface for git %ud\n", gid);
    return nullptr;
}
SDL_Surface *Tiled::DrawBackgroundAtlas()
{
    auto atlas = LoadTextureAtlas(_mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight);
    for (auto &group : _mapData->Groups)
    {
        if (group.Name == "background")
        {
            for (auto &groupLayer : group.Layers)
            {
                for (int y = 0; y < groupLayer.Height; ++y)
                {
                    for (int x = 0; x < groupLayer.Width; ++x)
                    {
                        auto index = (y * groupLayer.Width) + x;
                        auto tileGid = groupLayer.Data[index];
                        if (tileGid == 0)
                            continue;
                        auto tiledMapTileset = _mapData->GetTiledMapTilesetForGid(tileGid);
                        auto tileset = _mapData->GetTilesetForTiledMapTileset(tiledMapTileset);
                        auto tileSurface = GetSurfaceForGid(tileGid, tileset);
                        auto sourceRect = _mapData->GetSourceRectForGid(tileGid);
                        int dstX = x * _mapData->TileWidth;
                        int dstY = y * _mapData->TileHeight;
                        auto dstRect = SDL_Rect{dstX, dstY, sourceRect.w, sourceRect.h};
                        // // May need to adjust background image

                        BlitSurface(
                            tileSurface,
                            &sourceRect,
                            atlas,
                            &dstRect);
                    }
                }
            }
        }
    }
    return atlas;
}