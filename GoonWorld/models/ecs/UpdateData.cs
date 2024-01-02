// typedef struct geContextUpdateData
// {
//     double updateTime;

// } geContextUpdateData;

using System.Runtime.InteropServices;
namespace GoonEngine.Models;
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct UpdateData
{
    public double UpdateTime;
}