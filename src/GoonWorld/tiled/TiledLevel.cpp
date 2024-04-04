#include <GoonWorld/gnpch.hpp>
#include <GoonWorld/tiled/TiledLevel.hpp>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonEngine/SdlSurface.h>
#include <GoonPhysics/scene.h>
#include <GoonPhysics/body.h>
#include <nlohmann/json.hpp>
#include <GoonWorld/shared/Constants.hpp>

#include <glad/glad.h>
#include <GoonEngine/Texture2D.h>
#include <GoonEngine/TileSheet.h>
#include <GoonEngine/Shader.h>
using namespace GoonWorld;

TiledLevel::TiledLevel(const char *filename)
    : _loadedAtlas(0)
{
    _mapData = std::make_unique<TiledMap>(filename);
    _name = filename;
    LoadGravity();
    LoadSurfaces();
    LoadSolidObjects();
    LoadBgm();
    CreateBackgroundAtlas();
}
std::vector<TiledMap::TiledObject> TiledLevel::GetAllObjects()
{
    return _mapData->Objects;
}

TiledLevel::~TiledLevel()
{
    // DestroyTexture(_loadedAtlas);
    // geTexture2DFree(_loadedAtlas);
    //TODO need to clean texture
}

void TiledLevel::SetTextureAtlas()
{
    // auto rect = geRectangle{
    //     0,
    //     0,
    //     _mapData->Width * _mapData->TileWidth,
    //     _mapData->Height * _mapData->TileHeight,
    // };
    SetBackgroundAtlas(_loadedAtlas);
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
                auto texture = geTexture2DNew();
                geTexture2DGenerate(texture, surfacePath.c_str());
                _loadedTilesets.push_back({tile.Image, texture});
            }
        }
        else
        {
            auto surfacePath = AssetPrefix + TiledPrefix + tileset.Image;
            auto texture = geTexture2DNew();
            geTexture2DGenerate(texture, surfacePath.c_str());
            _loadedTilesets.push_back({tileset.Image, texture});
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
geTexture2D *TiledLevel::GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset)
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
// static unsigned int LoadAtlas(int width, int height)
// {
//     GLuint textureAtlas; // ID of the texture atlas

//     // Generate a new texture ID
//     glGenTextures(1, &textureAtlas);

//     // Bind the texture atlas
//     glBindTexture(GL_TEXTURE_2D, textureAtlas);

//     // Allocate memory for the texture atlas
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

//     // Set texture parameters (optional, adjust as needed)
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     return textureAtlas;
// }

void TiledLevel::CreateBackgroundAtlas()
{
    if (_loadedAtlas)
        return;

    _loadedAtlas = geTileSheetNew();


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
                        auto dstRect = geRectangle{dstX, dstY, sourceRect.w, sourceRect.h};

                        geTileSheetAddTile(_loadedAtlas, &dstRect, &sourceRect, tileSurface);

                    }
                }
            }
        }
    }
    geTileSheetBufferData(_loadedAtlas);
    // _loadedAtlas = CreateTextureFromSurface(atlas);
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

void TiledLevel::LoadBgm()
{
    for (auto &property : _mapData->Properties)
    {
        if (property.Name != "bgm")
            continue;
        auto gravityJson = nlohmann::json::parse(property.ValueJsonString);
        _bgmName = gravityJson["bgmName"];
        _bgmStart = gravityJson["loopBegin"];
        _bgmEnd = gravityJson["loopEnd"];
        _volume = gravityJson["volume"];
    }
}

std::string TiledLevel::GetNextLevel()
{
    for (auto &property : _mapData->Properties)
    {
        if (property.Name != "nextLevel")
            continue;
        return property.ValueJsonString;
    }
    return "";
}

void TiledLevel::RestartLevel()
{
    LoadSolidObjects();
}