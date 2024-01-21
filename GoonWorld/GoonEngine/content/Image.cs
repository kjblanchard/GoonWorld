namespace GoonEngine.Content;

public class Image
{
    private static Dictionary<string, Image> _loadedImages = new();
    public IntPtr LoadedImagePtr => _loadedData;
    private IntPtr _loadedData;
    private Image(string filename)
    {
        var fullFilepath = $"assets/img/{filename}.png";
        _loadedData = Api.Content.CreateTextureFromFile(fullFilepath);
        _loadedImages[filename] = this;
    }

    public static Image LoadImage(string filepath) => _loadedImages.TryGetValue(filepath, out var image) ? image : new Image(filepath);

}