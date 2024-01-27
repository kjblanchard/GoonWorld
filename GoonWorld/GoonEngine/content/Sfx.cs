namespace GoonEngine.Content;
public class Sfx
{
    private static Dictionary<string, Sfx> _loadedSfx = new();
    public IntPtr LoadedSfxPtr => _loadedData;
    private IntPtr _loadedData;
    private Sfx(string filename)
    {
        var fullFilepath = $"assets/audio/{filename}";
        _loadedData = Api.Sound.LoadSfxHelper(fullFilepath);
        _loadedSfx[filename] = this;
    }

    public static Sfx LoadSfx(string filepath) => _loadedSfx.TryGetValue(filepath, out var image) ? image : new Sfx(filepath);

}