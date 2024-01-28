using GoonEngine;
using GoonEngine.Objects;
using System.Reflection;



class Program
{

    public static Dictionary<string, Func<object, GameObject>> ObjectSpawnDictionary = new Dictionary<string, Func<object, GameObject>>
{
    { "Player", (data) => new Player(data) },
    { "Enemy", (data) => new Goomba(data) },
    { "DeathBox", (data) => new DeathBox(data) },
    // { "Brick", (data) => new Brick(data) },
};
    static void Main()
    {
        var game = new Game();
        Debug.Level = Debug.LogLevel.Error;
        game.Initialize();
        CreateAllAnimations();
        InitializePhysics();
        game.CurrentLevel = new Tiled("level1");
        game.Run();
    }

    private static void CreateAllAnimations()
    {

        var animatedClasses = AppDomain.CurrentDomain.GetAssemblies()
            .SelectMany(assembly => assembly.GetTypes())
            .Where(type => typeof(IAnimate).IsAssignableFrom(type))
            .ToList();
        foreach (var classType in animatedClasses)
        {
            MethodInfo methodInfo = classType.GetMethod("CreateAnimations", BindingFlags.Public | BindingFlags.Static);
            methodInfo?.Invoke(null, null);
        }

    }
    private static void InitializePhysics()
    {
        var scene = Api.Physics.Scene.gpInitScene();
        Api.Physics.Scene.geSetCurrentScene(scene);
        Api.Physics.Scene.gpSceneSetGravity(scene, 50);

    }
}
