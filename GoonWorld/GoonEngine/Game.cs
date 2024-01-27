using GoonEngine.Components;

namespace GoonEngine;

public class Game
{
    public ConfigManager Config { get; }
    public Sound Sound { get; }
    public static Game Global => _gameInstance;
    private static Game _gameInstance;
    private static Api.Engine.UpdateGameDelegate _updateFunc = Update;
    private static Api.Engine.DrawUpdateDelegate _drawFunc = Draw;
    public Tiled CurrentLevel;
    public bool Debug = false;
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

    public bool Initialize()
    {
        Api.Engine.GnInitializeEngine();
        Api.Engine.geGameSetUpdateFunc(_updateFunc);
        Api.Engine.geGameSetDrawFunc(_drawFunc);
        Api.Rendering.CreateWindowAndRenderer(Config.Config.windowConfig.windowSize.x, Config.Config.windowConfig.windowSize.y, Config.Config.windowConfig.title);
        Api.Sound.InitializeSound();
        return true;
    }
    public static void Update(double deltaTime)
    {
        long ticks = (long)(deltaTime * TimeSpan.TicksPerMillisecond);
        GameObject.DeltaTime = TimeSpan.FromTicks(ticks);
        PhysicsComponent.PhysicsUpdate();
        GameObject.UpdateAllGameObjects();
    }
    public static void Draw()
    {

        GameObject.DrawGameObjectBoxes();
        GameObject.DrawGameObjectDebugBoxes();
        if(_gameInstance.Debug && _gameInstance.CurrentLevel != null )
        {
            var color = new Color(0, 200, 0, 255);
            foreach(var solid in _gameInstance.CurrentLevel.StaticBodies)
            {
                var rect = new Rect(solid);
                Api.Rendering.DrawDebugRect(ref rect, ref color);
            }


        }
    }

    public void Run()
    {
        Api.Update.Play();
    }

}
