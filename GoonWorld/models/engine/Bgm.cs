using System.Runtime.InteropServices;

namespace GoonEngine;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Bgm
{
    public IntPtr bgm_name;  // C string represented as IntPtr
    public double loop_begin;
    public double loop_end;

    // Helper method to convert IntPtr to string
    public string GetBgmName()
    {
        return Marshal.PtrToStringAnsi(bgm_name) ?? "Error reading Bgm Name";
    }
}