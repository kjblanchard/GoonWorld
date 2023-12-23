using System.Runtime.InteropServices;
namespace GoonEngine;
public class Sound
{
    public Sound()
    {

    }
    ~Sound()
    {

    }
    private static Dictionary<string, IntPtr> LoadedBgms = new();

    public bool LoadBgm(string filename, float loopBegin, float loopEnd)
    {
        var bgmPtr = BgmLoad(filename, loopBegin, loopEnd);
        if (bgmPtr == IntPtr.Zero)
        {
            // Invalid bgm ptr
            return false;
        }
        LoadedBgms[filename] = bgmPtr;
        return true;
    }

    public void PlayBgm(string filename, float volume)
    {
        BgmPlay(LoadedBgms[filename], volume);
    }



    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr BgmLoad(string filename, float begin, float end);
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int BgmPlay(IntPtr bgm, float volume);


}