using GoonEngine;

class Program
{
    const int numGameObjects = 1;
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
            new GameObject();
            Console.WriteLine(GameObject.EntityToGameObjectDictionary.Count);
        }
        var locationSystem = new LocationSystem();
        ECS.UpdateSystems(IntPtr.Zero);
        game.Run();
    }
}
