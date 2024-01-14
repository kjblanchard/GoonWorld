namespace GoonEngine;

public class Game
{
    public ConfigManager Config { get; }
    public Sound Sound { get; }
    public static Game Global => _gameInstance;
    private static Game _gameInstance;
    private static Api.Engine.UpdateGameDelegate _updateFunc;
    public Game()
    {
        if (_gameInstance != null)
        {
            throw new Exception("Should only have one instance of game!");
        }
        _gameInstance = this;
        _updateFunc = Update;
        Api.Engine.GnInitializeEngine();
        Api.Engine.geGameSetUpdateFunc(_updateFunc);
        Config = new ConfigManager();
        Sound = new Sound(Config.Config.soundConfig);
    }
    ~Game()
    {

    }

    public bool CreateWindow()
    {
        Api.Rendering.CreateWindowAndRenderer(Config.Config.windowConfig.windowSize.x, Config.Config.windowConfig.windowSize.y, Config.Config.windowConfig.title);
        return true;
    }

    public bool Initialize()
    {
        ECS.Initialize();
        return true;
    }
    public void Update(double deltaTime)
    {
        long ticks = (long)(deltaTime * TimeSpan.TicksPerMillisecond);
        GameObject.DeltaTime = TimeSpan.FromTicks(ticks);
        GameObject.UpdateAllGameObjects();
    }

    public void Run()
    {
        Api.Update.Play();
    }


}
