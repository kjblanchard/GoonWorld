using TiledCS;
using System.Runtime.InteropServices;
using GoonEngine.Models;
namespace GoonEngine;

public class Tiled
{
    private const string _assetPrefix = "assets/";
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr LoadSurfaceFromFile(string filepath);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void BlitSurface(IntPtr srcSurface, ref Rect srcRect, IntPtr dstSurface, ref Rect dstRect);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr LoadTextureAtlas(int width, int height);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void SetBackgroundAtlas(IntPtr background, ref Rect rect);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr CreateTextureFromSurface(IntPtr surface);
    public List<BoundingBox> StaticBodies = new();

    public TiledMap LoadedMap;
    private Dictionary<string, IntPtr> LoadedTilesetImages = new();
    private void LoadBgm(string bgmName)
    {
        Game.Global.Sound.LoadBgm(bgmName);
        Game.Global.Sound.PlayBgm(bgmName);

    }
    public Tiled(string level)
    {
        var pathPrefix = "assets/tiled/";
        LoadedMap = new TiledMap(pathPrefix + level + ".tmx");
        var tilesets = LoadedMap.GetTiledTilesets(_assetPrefix + "tiled/");

        // Load BGM
        var properties = LoadedMap.Properties;
        foreach (var property in LoadedMap.Properties)
        {
            if (property.name == "bgmName")
            {
                LoadBgm(property.value);
            }

        }

        foreach (var group in LoadedMap.Groups)
        {
            if (group.name == "background")
            {
                var atlas = LoadTextureAtlas(LoadedMap.Width * LoadedMap.TileWidth, LoadedMap.Height * LoadedMap.TileHeight);
                foreach (var layer in group.layers)
                {
                    for (int y = 0; y < layer.height; y++)
                    {
                        for (int x = 0; x < layer.width; x++)
                        {
                            var index = (y * layer.width) + x; // Assuming the default render order is used which is from right to bottom
                            var tileGid = layer.data[index]; // The tileset tile index
                            if (tileGid == 0)
                                continue;
                            var tilesetMap = LoadedMap.GetTiledMapTileset(tileGid);
                            var tileset = tilesets[tilesetMap.firstgid];
                            var tiledTile = LoadedMap.GetTiledTile(tilesetMap, tileset, tileGid);
                            IntPtr loadedTileset = IntPtr.Zero;
                            var dstX = x * LoadedMap.TileWidth;
                            var dstY = y * LoadedMap.TileHeight;
                            if (tiledTile == null)
                            {
                                // this is a tile, use regular x for destination
                                loadedTileset = GetImageFromFilepath(tileset.Image.source);
                            }
                            else
                            {
                                // This is an image tile.
                                loadedTileset = GetImageFromFilepath(tiledTile.image.source);
                                dstY -= LoadedMap.TileHeight;
                            }
                            var srcRect = new Rect(LoadedMap.GetSourceRect(tilesetMap, tileset, tileGid));
                            var dstRect = new Rect(
                                dstX,
                                dstY,
                                srcRect.Width,
                                srcRect.Height
                            );
                            BlitSurface(loadedTileset, ref srcRect, atlas, ref dstRect);
                        }
                    }

                }
                var bgRect = new Rect(0, 0, LoadedMap.TileWidth * LoadedMap.Width, LoadedMap.TileHeight * LoadedMap.Height);
                var texPtr = CreateTextureFromSurface(atlas);
                SetBackgroundAtlas(texPtr, ref bgRect);
            }

        }
        foreach (var layer in LoadedMap.Layers)
        {
            if (layer.type == TiledLayerType.ObjectLayer)
            {
                if (layer.name == "entities")
                {
                    foreach (var entityObject in layer.objects)
                    {
                        if (Program.ObjectSpawnDictionary.ContainsKey(entityObject.type))
                        {
                            var newGo = Program.ObjectSpawnDictionary[entityObject.type](entityObject);
                        }
                    }

                }
                else if (layer.name == "solid")
                {
                    foreach (var entityObject in layer.objects)
                    {
                        var x = entityObject.x;
                        var y = entityObject.y;
                        Point[] points = new Point[entityObject.polygon.points.Length / 2];
                        for (int i = 0; i < points.Length; i++)
                        {
                            points[i] = new Point(entityObject.polygon.points[i * 2], entityObject.polygon.points[(i * 2) + 1]);
                        }
                        var minX = x + points.Min(p => p.X);
                        var minY = y + points.Min(p => p.Y);
                        var maxX = x + points.Max(p => p.X);
                        var maxY = y + points.Max(p => p.Y);
                        BoundingBox box = new BoundingBox(minX, minY, maxX - minX, maxY - minY);
                        StaticBodies.Add(box);
                        var body = Api.Physics.Body.gpBodyNew(box);
                        Api.Physics.Scene.gpSceneAddStaticBody(body);
                    }

                }
            }

        }
    }

    private IntPtr GetImageFromFilepath(string filepath)
    {
        if (LoadedTilesetImages.TryGetValue(filepath, out var loadedPtr))
        {
            return loadedPtr;
        }
        var loadPath = _assetPrefix + string.Join('/', filepath.Split('/').Skip(1));
        return LoadedTilesetImages[filepath] = LoadSurfaceFromFile(loadPath);
    }

    private void CreateBlittedTilemap(TiledMap map)
    {


    }

}

