using TiledCS;

public class Tiled
{
    public TiledMap LoadedMap;
    // private Dictionary<int, LoadedTileset> LoadedTilesetImages = new();
    private List<LoadedTileset> LoadedTilesetImages = new();

    private struct LoadedTileset
    {
        public IntPtr LoadedImage;
        public TiledTileset Tileset;
        public int FirstGid;
        public bool ImageTileset;

        public LoadedTileset(TiledTileset tileset, int firstGid)
        {
            LoadedImage = IntPtr.Zero;
            FirstGid = firstGid;
            Tileset = tileset;
            ImageTileset = tileset.Tiles.Any() ? true : false;
        }

    }
    public Tiled()
    {
        var pathPrefix = "assets/tiled/";
        LoadedMap = new TiledMap(pathPrefix + "level1.tmx");
        foreach (var tileset in LoadedMap.Tilesets)
        {
            var loadedTileset = new TiledTileset(pathPrefix + tileset.source);
            LoadedTilesetImages.Add(new LoadedTileset(loadedTileset, tileset.firstgid));
        }
        LoadedTilesetImages = LoadedTilesetImages.OrderBy(x => x.FirstGid).ToList();

        foreach (var group in LoadedMap.Groups)
        {
            // Create a tilemap
            if (group.name == "background")
            {
                foreach (var layer in group.layers)
                {
                    Console.WriteLine(layer.name);
                    foreach (var tileGid in layer.data)
                    {
                        if (tileGid == 0)
                            continue;
                        var tileTileset = LoadedTilesetImages.OrderByDescending(tileset => tileset.FirstGid)
                                                                .FirstOrDefault(tileset => tileset.FirstGid <= tileGid);
                        Console.WriteLine($"Tile num {tileGid} is in tileset {tileTileset.Tileset.Name} and the tilesets first gid is {tileTileset.FirstGid}");
                        if (tileTileset.ImageTileset)
                        {
                            // We should blit the image
                        }
                        else
                        {
                            // We should blit the tile
                        }
                    }

                }
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

    private void CreateBlittedTilemap(TiledMap map)
    {


    }

}

