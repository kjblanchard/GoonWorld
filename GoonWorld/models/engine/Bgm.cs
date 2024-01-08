using System.Runtime.InteropServices;

namespace GoonEngine;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Bgm
{
    public IntPtr bgm_name;
    public double loop_begin;
    public double loop_end;

    public string BgmName => Marshal.PtrToStringAnsi(bgm_name) ?? "Error reading Bgm Name";
}