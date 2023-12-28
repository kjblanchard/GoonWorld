using System.Runtime.InteropServices;
namespace GoonEngine;
interface IComponent
{
    /// <summary>
    /// </summary>
    public IntPtr ComponentPtr { get; set; }
    public IntPtr ECSComponentPtr { get; set; }

}