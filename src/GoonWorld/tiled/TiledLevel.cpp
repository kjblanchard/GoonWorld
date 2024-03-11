#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonPhysics/scene.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/shared/Constants.hpp>
using namespace GoonWorld;

TiledLevel::TiledLevel(const char *filename)
: _loadedAtlas (nullptr)
{
    _mapData = std::make_unique<TiledMap>(filename);
    _name = filename;
    LoadGravity();
    LoadSurfaces();
    LoadSolidObjects();
    CreateBackgroundAtlas();
}
std::vector<TiledMap::TiledObject> TiledLevel::GetAllObjects()
{
    return _mapData->Objects;
}

TiledLevel::~TiledLevel()
{
    DestroyTexture(_loadedAtlas);
}

void TiledLevel::SetTextureAtlas()
{
    auto rect = SDL_Rect{
        0,
        0,
        _mapData->Width * _mapData->TileWidth,
        _mapData->Height * _mapData->TileHeight,
    };
    SetBackgroundAtlas(_loadedAtlas, &rect);
}

void TiledLevel::LoadSurfaces()
{
    for (auto &tileset : _mapData->Tilesets)
    {
        if (tileset.Type == TilesetType::Image)
        {
            for (auto &tile : tileset.Tiles)
            {
                auto surfacePath = AssetPrefix + TiledPrefix + tile.Image;
                // auto surface = LoadSurfaceFromFile(surfacePath.c_str());
                auto surface = (SDL_Surface *)Content::LoadContent(ContentTypes::Surface, surfacePath.c_str());
                _loadedTilesets.push_back({tile.Image, surface});
            }
        }
        else
        {
            auto surfacePath = AssetPrefix + TiledPrefix + tileset.Image;
            // auto surface = LoadSurfaceFromFile(surfacePath.c_str());
            auto surface = (SDL_Surface *)Content::LoadContent(ContentTypes::Surface, surfacePath.c_str());
            _loadedTilesets.push_back({tileset.Image, surface});
        }
    }
}
void TiledLevel::LoadSolidObjects()
{
    for (auto &solid : _mapData->SolidObjects)
    {
        auto box = gpBBNew(solid.X, solid.Y, solid.Width, solid.Height);
        auto body = gpBodyNewStatic(box);
        gpSceneAddStaticBody(body);
    }
}
SDL_Surface *TiledLevel::GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset)
{
    if (tileset->Type == TilesetType::Image)
    {
        for (auto &tile : tileset->Tiles)
        {
            if (tile.Id + tileset->FirstGid == gid)
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
            if (surface.first == tileset->Image)
                return surface.second;
        }
    }
    printf("Could not find loaded surface for git %ud\n", gid);
    return nullptr;
}
void TiledLevel::CreateBackgroundAtlas()
{
    // TODO this should be reworked, but for now don't create one if it already exists, for reloading
    if (_loadedAtlas)
        return;
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
                        auto tiledMapTileset = _mapData->GetGidTiledMapTileset(tileGid);
                        auto tileset = _mapData->GetTiledMapTilesetTileset(tiledMapTileset);
                        auto tileSurface = GetSurfaceForGid(tileGid, tileset);
                        auto sourceRect = _mapData->GetGidSourceRect(tileGid);
                        auto dstX = x * _mapData->TileWidth;
                        auto dstY = y * _mapData->TileHeight;
                        // Adjust background image as tiled draws it from the opposite end.
                        if (tileset->Type == TilesetType::Image)
                        {
                            dstY -= (sourceRect.h - _mapData->TileHeight);
                        }
                        auto dstRect = SDL_Rect{dstX, dstY, sourceRect.w, sourceRect.h};
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
    _loadedAtlas = CreateTextureFromSurface(atlas);
}
void TiledLevel::LoadGravity()
{
    for (auto &property : _mapData->Properties)
    {
        if (property.Name != "gravity")
            continue;
        auto gravityJson = nlohmann::json::parse(property.ValueJsonString);
        _gravity.x = gravityJson["x"];
        _gravity.y = gravityJson["y"];
    }
}

void TiledLevel::RestartLevel()
{
    LoadSolidObjects();
    // CreateBackgroundAtlas();
}