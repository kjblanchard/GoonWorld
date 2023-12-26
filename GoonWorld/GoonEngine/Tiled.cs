using TiledCS;
using System.Runtime.InteropServices;
using System.Reflection.Metadata;
namespace GoonEngine;

public class Tiled
{
    private const string _assetPrefix = "assets/";
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr LoadSurfaceFromFile(string filepath);
    [DllImport("../build/lib/libSupergoonEngine")]
    // private static extern void BlitSurface(IntPtr atlasSurface, IntPtr tileSurface, IntPtr dstRect, IntPtr srcRect);
    private static extern void BlitSurface( IntPtr srcSurface, ref SDL_Rect srcRect, IntPtr dstSurface, ref SDL_Rect dstRect);
    // private static extern void BlitSurface(IntPtr atlasSurface, IntPtr tileSurface, ref SDL_Rect dstRect, ref SDL_Rect srcRect);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr LoadTextureAtlas(int width, int height);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void SetBackgroundAtlas(IntPtr background, ref SDL_Rect rect);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr CreateTextureFromSurface(IntPtr surface);

    public TiledMap LoadedMap;
    private Dictionary<string, IntPtr> LoadedTilesetImages = new();
    public Tiled()
    {
        var pathPrefix = "assets/tiled/";
        LoadedMap = new TiledMap(pathPrefix + "level1.tmx");
        var tilesets = LoadedMap.GetTiledTilesets(_assetPrefix + "tiled/");

        foreach (var group in LoadedMap.Groups)
        {
            if (group.name == "background")
            // We should create a atlas for the background and blit it all
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
                            // var tileGid = layer.data[x];
                            if (tileGid == 0)
                                continue;
                            var tilesetMap = LoadedMap.GetTiledMapTileset(tileGid);
                            var tileset = tilesets[tilesetMap.firstgid];
                            var tiledTile = LoadedMap.GetTiledTile(tilesetMap, tileset, tileGid);
                            IntPtr loadedTileset = IntPtr.Zero;
                            var dstX = x;
                            var dstY = y;
                            if(tiledTile == null)
                            {
                                // this is a tile, use regular x for destination
                                loadedTileset =  GetImageFromFilepath(tileset.Image.source);
                            }
                            else
                            {
                                // This is an image tile.
                                loadedTileset =  GetImageFromFilepath(tiledTile.image.source);
                            }
                            var srcRect = new SDL_Rect(LoadedMap.GetSourceRect(tilesetMap, tileset, tileGid));
                            var dstRect = new SDL_Rect(
                                // x * tileset.TileWidth,
                                x * LoadedMap.TileWidth,
                                y * LoadedMap.TileHeight,
                                srcRect.width,
                                srcRect.height
                            );
                            Console.WriteLine($"Going to blit at dest {dstRect.x}:{dstRect.y}:{dstRect.width}:{dstRect.height} from source {srcRect.x}:{srcRect.y}:{srcRect.width}:{srcRect.height} for gid {tileGid}");
                            // BlitSurface(atlas, loadedTileset, ref dstRect, ref srcRect);
                            BlitSurface(loadedTileset,ref srcRect, atlas, ref dstRect);
                        }
                    }

                }
                var bgRect = new SDL_Rect(0, 0, 512, 288);
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
                    Console.WriteLine("Entities found");

                }
                else if (layer.name == "solid")
                {
                    Console.WriteLine("Solids found");

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

