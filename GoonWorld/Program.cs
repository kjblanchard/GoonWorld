using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using GoonEngine;

class Program
{


    static void Main()
    {
        var game = new Game();
        game.Initialize();
        game.CreateWindow();
        game.Sound.LoadBgm("rangers");
        game.Sound.PlayBgm("rangers");
        var level1 = new Tiled();
        for (int i = 0; i < 200; i++)
        {
            var go = new GameObject();
            Console.WriteLine(GameObject.EntityToGameObjectDictionary.Count);
            var entId = GameObject.EntityToGameObjectDictionary[go.Entity];
            Console.WriteLine(entId.Entity);
            var thing = go.HasComponent(0);
            var thing2 = go.HasComponent(1);
        }
        game.Run();
    }
}
