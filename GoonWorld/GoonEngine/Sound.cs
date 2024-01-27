using GoonEngine.Content;

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
        var bgmPtr = Api.Sound.BgmLoad(foundMusic.name, foundMusic.startLoop, foundMusic.endLoop);
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
            Api.Sound.BgmPlay(bgm, volumeOverride ?? _soundConfig.musicVolume);
        }
        else
        {
            Debug.WarnMessage($"BGM is not loaded {filename}");
        }
    }
    public void PlaySfx(Sfx sfx, float? volumeOverride = null)
    {
        Api.Sound.PlaySfxOneShot(sfx.LoadedSfxPtr, volumeOverride ?? _soundConfig.musicVolume);

    }





}