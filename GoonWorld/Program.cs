using GoonEngine;
using GoonEngine.Objects;



class Program
{

    public static Dictionary<string, Func<object, GameObject>> ObjectSpawnDictionary = new Dictionary<string, Func<object, GameObject>>
{
    { "Player", (data) => new Player(data) },
    { "Enemy", (data) => new Goomba(data) },
};

    static void Main()
    {
        var game = new Game();
        Debug.Level = Debug.LogLevel.Error;
        game.Initialize();
        var scene = Api.Physics.Scene.gpInitScene();
        Api.Physics.Scene.geSetCurrentScene(scene);
        Api.Physics.Scene.gpSceneSetGravity(scene, 50);
        game.CurrentLevel = new Tiled("level1");

        Player.CreateAnimations();
        game.Run();
    }
}
