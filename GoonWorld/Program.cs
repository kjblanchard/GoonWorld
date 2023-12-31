using GoonEngine;


class Program
{
    const int numGameObjects = 2;
    static void Main()
    {
        var game = new Game();
        game.Initialize();
        game.CreateWindow();
        game.Sound.LoadBgm("rangers");
        game.Sound.PlayBgm("rangers");
        var level1 = new Tiled();
        for (int i = 0; i < numGameObjects; i++)
        {
            var go = new GameObject();
            go.Location.X = i;
            go.Location.Y = i;
            Console.WriteLine(GameObject.EntityToGameObjectDictionary.Count);
        }
        var locationSystem = new LocationSystem();
        var updateSystem = new ScriptSystem();
        ECS.UpdateSystems(IntPtr.Zero);
        game.Run();
    }
}
