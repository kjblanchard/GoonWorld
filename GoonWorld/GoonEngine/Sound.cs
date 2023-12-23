using System.Runtime.InteropServices;
using System.Linq;
using System.Collections.Generic;
using System;
namespace GoonEngine;
public class Sound
{
    ConfigManager.SoundConfig _soundConfig;
    private static Dictionary<string, IntPtr> LoadedBgms = new();
    public Sound(ConfigManager.SoundConfig soundConfig)
    {
        _soundConfig = soundConfig;
    }
    ~Sound()
    {

    }

    public bool LoadBgm(string title)
    {
        var foundMusic = _soundConfig.musicToLoad.FirstOrDefault(music => music.title == title);
        if(foundMusic == null)
        {
            // Could not find debug
            return false;
        }
        var bgmPtr = BgmLoad(foundMusic.name, foundMusic.startLoop, foundMusic.endLoop);
        if (bgmPtr == IntPtr.Zero)
        {
            // Invalid bgm ptr
            return false;
        }
        LoadedBgms[title] = bgmPtr;
        return true;
    }

    public void PlayBgm(string filename, float? volumeOverride = null)
    {
        BgmPlay(LoadedBgms[filename], volumeOverride ?? _soundConfig.musicVolume);
    }



    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr BgmLoad(string filename, float begin, float end);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int BgmPlay(IntPtr bgm, float volume);


}