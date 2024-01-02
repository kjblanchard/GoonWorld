using GoonEngine;
using GoonEngine.Objects;



class Program
{

public static Dictionary<string, Func<object, GameObject>> ObjectSpawnDictionary = new Dictionary<string, Func<object, GameObject>>
{
    { "Player", (data) => new Player(data) },
};

    static void Main()
    {
        var game = new Game();
        game.Initialize();
        game.CreateWindow();
        // game.Sound.LoadBgm("rangers");
        // game.Sound.PlayBgm("rangers");
        var level1 = new Tiled();
        var locationSystem = new LocationSystem();
        var updateSystem = new ScriptSystem();
        var drawSystem = new DrawSystem();
        ECS.UpdateSystems(IntPtr.Zero);
        game.Run();
    }
}
