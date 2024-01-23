using System.Text.Json;
using System.Text.Json.Serialization;
namespace GoonEngine;
public class ConfigManager
{
    public GameConfig Config;
    public ConfigManager(string configFilepath = "assets/config/appsettings.json")
    {
        string jsonContent = File.ReadAllText(configFilepath);
        Config = JsonSerializer.Deserialize<GameConfig>(jsonContent);
    }
    // [JsonSerializable(typeof(JsonElement))]
    public class GameConfig
    {
        public WindowConfig windowConfig { get; set; }
        public GraphicsConfig graphicsConfig { get; set; }
        public SoundConfig soundConfig { get; set; }
        public List<List<List<int>>> keyboardConfig { get; set; }
        public List<List<int>> controllerConfig { get; set; }
        [JsonPropertyName("animation")]
        public Dictionary<string, AnimationFile> Animations { get; set; }
        // public Animation Animation {get; set;}
    }

    public class GraphicsConfig
    {
        public bool vsync { get; set; }
        public int fps { get; set; }
    }


    public class SoundConfig
    {
        public float musicVolume { get; set; }
        public MusicToLoad[] musicToLoad { get; set; }
    }
    public class MusicToLoad
    {
        public string title { get; set; }
        public string name { get; set; }
        public float startLoop { get; set; }
        public float endLoop { get; set; }
    }

    public class WindowConfig
    {
        public WindowSize windowSize { get; set; }
        public string title { get; set; }
    }

    public class WindowSize
    {
        public uint x { get; set; }
        public uint y { get; set; }
    }

    public class AnimationFile
    {
        [JsonPropertyName("default")]
        public string Default { get; set; }
        // public <string, Animation> animations { get; set; }
        public List<Animation> animations { get; set; }
        public class Animation
        {
            public string name { get; set; }
            public bool looping { get; set; }
        }
    }

}