using System.Runtime.InteropServices;
namespace GoonEngine;

public class Game
{
    public ConfigManager Config { get; }
    public Sound Sound { get; }
    public static Game Global => _gameInstance;
    private static Game _gameInstance;
    public Game()
    {
        if (_gameInstance != null)
        {
            throw new Exception("Should only have one instance of game!");
        }
        _gameInstance = this;
        Config = new ConfigManager();
        Sound = new Sound(Config.Config.soundConfig);
    }
    ~Game()
    {

    }

    public bool CreateWindow()
    {
        CreateWindowAndRenderer(Config.Config.windowConfig.windowSize.x, Config.Config.windowConfig.windowSize.y, Config.Config.windowConfig.title);
        return true;
    }

    public bool Initialize()
    {
        ECS.Initialize();
        return true;
    }

    public void Run()
    {
        Play();
    }


    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int CreateWindowAndRenderer(uint width, uint height, string windowTitle);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void Play();
}
