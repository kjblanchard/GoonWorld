using System.Runtime.InteropServices;
using GoonEngine;

class Program
{

    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern void Play();

    static void Main()
    {
        var game = new Game();
        game.CreateWindow();
        var sound = new Sound();
        var soundPath = "assets/audio/test.ogg";
        sound.LoadBgm (soundPath, 20.397f, 43.08f);
        sound.PlayBgm(soundPath, game._config.Config.soundConfig.musicVolume);
        Play();
    }
}
