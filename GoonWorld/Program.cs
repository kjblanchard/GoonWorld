using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

class Program
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Bgm
    {
        public IntPtr bgm_name;  // C string represented as IntPtr
        public double loop_begin;
        public double loop_end;

        // Helper method to convert IntPtr to string
        public string GetBgmName()
        {
            return Marshal.PtrToStringAnsi(bgm_name);
        }
    }

    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern void Play();
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int LuaCreateSdlWindow(uint width, uint height);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr BgmLoad(string filename, float begin, float end);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int BgmPlay(IntPtr bgm, float volume);

    static void Main()
    {
        Console.WriteLine("Calling function from DLL:");
        LuaCreateSdlWindow(400, 200);
        IntPtr bgmPtr = BgmLoad("assets/audio/test.ogg", 20.397f, 43.08f);
        Bgm bgm = Marshal.PtrToStructure<Bgm>(bgmPtr);
        Console.WriteLine("We should print and play " + bgm.GetBgmName());
        BgmPlay(bgmPtr, 1.0f);
        Play();
    }
}
