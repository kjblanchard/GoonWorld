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
        if (foundMusic == null)
        {
            Debug.WarnMessage($"Could no find {title} in appsettings");
            return false;
        }
        var bgmPtr = BgmLoad(foundMusic.name, foundMusic.startLoop, foundMusic.endLoop);
        if (bgmPtr == IntPtr.Zero)
        {
            Debug.WarnMessage($"Could no load {foundMusic.name} ");
            return false;
        }
        LoadedBgms[title] = bgmPtr;
        return true;
    }

    public void PlayBgm(string filename, float? volumeOverride = null)
    {
        if (LoadedBgms.TryGetValue(filename, out var bgm))
        {
            BgmPlay(bgm, volumeOverride ?? _soundConfig.musicVolume);
        }
        else
        {
            Debug.WarnMessage($"BGM is not loaded {filename}");
        }
        // BgmPlay(LoadedBgms[filename], volumeOverride ?? _soundConfig.musicVolume);
    }



    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr BgmLoad(string filename, float begin, float end);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int BgmPlay(IntPtr bgm, float volume);


}