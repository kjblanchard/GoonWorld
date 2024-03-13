#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonEngine/point.h>
#include <GoonWorld/shared/Constants.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace GoonWorld;

const TiledMap::TiledMapTileset *const TiledMap::GetGidTiledMapTileset(int gid)
{
    TiledMap::TiledMapTileset *tiledMapTileset = nullptr;
    for (auto tileset = TiledmapTilesets.begin(); tileset != TiledmapTilesets.end(); ++tileset)
    {
        if (gid >= tileset->FirstGid)
        {
            tiledMapTileset = &(*tileset);
        }
        else
            break;
    }
    if (!tiledMapTileset)
    {
        printf("Could not find tileset for git %ud\n", gid);
        return nullptr;
    }
    return tiledMapTileset;
}

const TiledMap::Tileset *const TiledMap::GetTiledMapTilesetTileset(const TiledMapTileset *tiledMapTileset)
{
    for (auto tileset = Tilesets.begin(); tileset != Tilesets.end(); ++tileset)
    {
        auto fullname = tileset->Name + ".tsj";
        if (tiledMapTileset->Source == fullname)
            return &(*tileset);
    }
    return nullptr;
}

geRectangle TiledMap::GetSourceRectForGidWithTileset(int gid, const TiledMap::Tileset *tileset)
{
    if (tileset->Type == TilesetType::Image)
    {
        for (auto &tile : tileset->Tiles)
        {
            if (tile.Id + tileset->FirstGid == gid)
            {
                return geRectangle{0, 0, tile.ImageWidth, tile.ImageHeight};
            }
        }
    }
    else
    {
        // Subtract 1 from gid, since 0 is counted as nothing in tiled.
        --gid;
        int x = (gid % tileset->Columns) * tileset->TileWidth;
        int y = (gid / tileset->Columns) * tileset->TileHeight;
        return geRectangle{x, y, tileset->TileWidth, tileset->TileHeight};
    }
    return geRectangle{0, 0, 0, 0};
}
geRectangle TiledMap::GetGidSourceRect(int gid)
{
    auto tiledMapTileset = GetGidTiledMapTileset(gid);
    auto tileset = GetTiledMapTilesetTileset(tiledMapTileset);
    return GetSourceRectForGidWithTileset(gid, tileset);
    // return GetSurfaceForGid(gid, )
}

TiledMap::TiledMap(std::string filename)
{
    auto pathPrefix = AssetPrefix + TiledPrefix + filename + ".tmj";
    std::ifstream file(pathPrefix);
    json data = json::parse(file);
    auto &tilesets = data["tilesets"];
    Width = data["width"];
    Height = data["height"];
    TileWidth = data["tilewidth"];
    TileHeight = data["tileheight"];
    for (auto &tileset : data["tilesets"])
    {
        auto tilemapTileset = TiledMapTileset();
        tilemapTileset.FirstGid = tileset["firstgid"];
        tilemapTileset.Source = tileset["source"];
        TiledmapTilesets.push_back(tilemapTileset);
    }
    for (auto &tiledmapTileset : TiledmapTilesets)
    {
        auto tileset = Tileset();
        tileset.FirstGid = tiledmapTileset.FirstGid;
        std::string tilesetPrefix = AssetPrefix + TiledPrefix + tiledmapTileset.Source;
        std::ifstream file(tilesetPrefix);
        json tilesetData = json::parse(file);
        // read the tileset file, and load info about it here.
        tileset.Name = tilesetData["name"];
        tileset.TileHeight = tilesetData["tileheight"];
        tileset.TileWidth = tilesetData["tilewidth"];
        tileset.Columns = tilesetData["columns"];
        auto tilesetTilesIterator = tilesetData.find("tiles");
        if (tilesetTilesIterator != tilesetData.end())
        {
            tileset.Type = TilesetType::Image;
            for (auto &tilesetTileJson : *tilesetTilesIterator)
            {
                auto tile = Tile();
                tile.Id = tilesetTileJson["id"];
                tile.ImageWidth = tilesetTileJson["imagewidth"];
                tile.ImageHeight = tilesetTileJson["imageheight"];
                tile.Image = tilesetTileJson["image"];
                tileset.Tiles.push_back(tile);
            }
        }
        else
        {
            tileset.Type = TilesetType::Tile;
            tileset.Image = tilesetData["image"];
        }
        Tilesets.push_back(tileset);
    }

    for (auto &layerJson : data["layers"])
    {
        std::string layerType = layerJson["type"];
        if (layerType == "group")
        {
            auto layerGroup = LayerGroup();
            layerGroup.Name = layerJson["name"];
            for (auto &groupLayer : layerJson["layers"])
            {
                auto layerGroupLayer = Layer();
                layerGroupLayer.Name = groupLayer["name"];
                layerGroupLayer.Height = groupLayer["height"];
                layerGroupLayer.Width = groupLayer["width"];
                layerGroupLayer.Data = groupLayer["data"].get<std::vector<int>>();
                layerGroup.Layers.push_back(layerGroupLayer);
            }
            Groups.push_back(layerGroup);
        }
        else if (layerType == "tilelayer")
        {
            auto layer = Layer();
            layer.Name = layerJson["name"];
            layer.Height = layerJson["height"];
            layer.Width = layerJson["width"];
            layer.Data = layerJson["data"].get<std::vector<int>>();
            Layers.push_back(layer);
        }
        else if (layerType == "objectgroup")
        {
            auto layer = Layer();
            auto name = layerJson["name"];
            if (name == "entities")
            {
                for (auto &objectJson : layerJson["objects"])
                {
                    auto object = TiledObject();
                    object.Id = objectJson["id"];
                    object.Height = objectJson["height"];
                    object.Width = objectJson["width"];
                    object.ObjectType = objectJson["type"];
                    object.X = objectJson["x"];
                    object.Y = objectJson["y"];
                    for (auto &property : objectJson["properties"])
                    {
                        // TODO use union in property class
                        auto prop = TiledProperty();
                        prop.Name = property["name"];
                        prop.PropertyType = property["propertytype"];
                        prop.ValueInt = property["value"];
                        object.Properties.push_back(prop);
                    }
                    Objects.push_back(object);
                }
                Layers.push_back(layer);
            }
            else if (name == "solid")
            {
                for (auto &objectJson : layerJson["objects"])
                {
                    auto object = TiledObject();
                    object.Id = objectJson["id"];
                    object.X = objectJson["x"];
                    object.Y = objectJson["y"];
                    // int pointsLength = objectJson["polygon"].size() / 2;
                    std::vector<Point> points;
                    // for (auto i = 0; i < pointsLength; ++i)
                    for (auto &point : objectJson["polygon"])
                    {
                        // points.push_back(SDL_Point{objectJson["polygon"][i * 2], objectJson["polygon"][i * 2 + 1]});
                        points.push_back(Point{point["x"], point["y"]});
                    }
                    int minX = object.X + (*std::min_element(points.begin(), points.end(), [](const Point &a, const Point &b)
                                                             { return a.x < b.x; }))
                                              .x;

                    int minY = object.Y + (*std::min_element(points.begin(), points.end(), [](const Point &a, const Point &b)
                                                             { return a.y < b.y; }))
                                              .y;

                    int maxX = object.X + (*std::max_element(points.begin(), points.end(), [](const Point &a, const Point &b)
                                                             { return a.x < b.x; }))
                                              .x;

                    int maxY = object.Y + (*std::max_element(points.begin(), points.end(), [](const Point &a, const Point &b)
                                                             { return a.y < b.y; }))
                                              .y;
                    object.X = minX;
                    object.Y = minY;
                    object.Width = maxX - minX;
                    object.Height = maxY - minY;
                    SolidObjects.push_back(object);
                }
            }
        }
    }
    for (auto &propertyJson : data["properties"])
    {
        TiledProperty property;
        property.Name = propertyJson["name"];
        property.PropertyType = propertyJson["propertytype"];
        property.ValueJsonString = propertyJson["value"].dump();
        Properties.push_back(property);
    }
}