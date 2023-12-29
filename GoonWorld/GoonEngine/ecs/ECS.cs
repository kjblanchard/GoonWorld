using System.ComponentModel.DataAnnotations;
using System.Runtime.InteropServices;
namespace GoonEngine;

public static class ECS
{
    public static void Initialize()
    {
        _context = geContextNew();
    }
    private static IntPtr _context = IntPtr.Zero;
    public static IntPtr NewComponent(int type, IntPtr componentData) => geContextComponentNew(_context, type, componentData);
    public static IntPtr NewEntity() => geContextEntityNew(_context);
    public static IntPtr GetComponentsOfType(int type) => geContextGetComponentArrayByType(_context, type);
    public static int GetNumComponentsOfType(int type) => geContextGetComponentArrayCountByType(_context, type);
    public static void UpdateSystems(IntPtr data) => geContextUpdate(_context, data);

    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr geContextNew();
    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern IntPtr geContextEntityNew(IntPtr context);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geContextComponentNew(IntPtr context, int type, IntPtr data);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern IntPtr geContextGetComponentArrayByType(IntPtr context, int type);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern int geContextGetComponentArrayCountByType(IntPtr context, int type);
    [DllImport("../build/lib/libSupergoonEngine")]
    public static extern void geContextUpdate(IntPtr context, IntPtr data);
    public static class Component
    {
        public unsafe static T? GetComponentOfType<T>(IntPtr ecsComponentPtr)
        {
            var componenetData = (Models.Component*)ecsComponentPtr;
            return Marshal.PtrToStructure<T>(componenetData->Data);
            // var componentData = Marshal.PtrToStructure<Models.Component>(ecsComponentPtr);
            // return Marshal.PtrToStructure<T>(componentData.Data);
        }
    }
    public static class Entity
    {
        public static IntPtr GetEntityPtrById(int id) => geGetEntityById(_context, id);

        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int geEntityAddComponent(IntPtr entity, IntPtr component);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern bool geEntityHasComponent(IntPtr entity, uint componentBit);
        [return: MarshalAs(UnmanagedType.I1)]
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern IntPtr geEntityGetComponent(IntPtr entity, uint componentBit);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int geEntityRemoveComponent(IntPtr entity, uint componentBit);
        [DllImport("../build/lib/libSupergoonEngine")]
        private static extern IntPtr geGetEntityById(IntPtr context, int id);

    }

    public class System
    {
        public static int NewSystem(SystemDelegate system, int systemType) => geContextSystemNew(_context, system, systemType);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void SystemDelegate(ref IntPtr context, int type, IntPtr data);
        [DllImport("../build/lib/libSupergoonEngine")]
        private static extern int geContextSystemNew(IntPtr context, SystemDelegate system, int systemType);

    }
}
