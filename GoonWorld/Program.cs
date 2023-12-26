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
        game.Run();
    }
}
