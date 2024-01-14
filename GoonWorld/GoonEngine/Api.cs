using System.Runtime.InteropServices;
using GoonEngine.Models;
namespace GoonEngine;
public static class Api
{
    public static class Engine
    {

        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int GnInitializeEngine();
        public delegate void UpdateGameDelegate(double deltaTime);

        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void geGameSetUpdateFunc(UpdateGameDelegate updateFunc);
        public delegate void DrawUpdateDelegate();
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void geGameSetDrawFunc(DrawUpdateDelegate drawFunc);
        public static class Input
        {
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern bool geKeyHeldDown(int key);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern bool geKeyJustPressed(int key);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern bool geKeyJustReleased(int key);
        }
    }

    public static class Physics
    {
        public static class Scene
        {

            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gpInitScene();
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void gpSceneSetGravity(IntPtr scene, float gravity);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern int gpSceneAddBody(IntPtr body);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern int gpSceneAddStaticBody(IntPtr body);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void gpSceneUpdate(IntPtr body, float gameTime);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gpSceneGetBody(int bodyRef);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void geSetCurrentScene(IntPtr scene);
        }
        public static class Body
        {

            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gpBodyNew(BoundingBox boundingBox);
        }
    }
    public static class Debug
    {
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int InitializeDebugLogFile();
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int CloseDebugLogFile();
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void LogDebug(string log);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void LogInfo(string log);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void LogWarn(string log);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void LogError(string log);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void SetLogLevel(int newLevel);

    }
    public static class Update
    {
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void Play();
    }
    public static class Rendering
    {
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern int CreateWindowAndRenderer(uint width, uint height, string windowTitle);
        public delegate void DrawUpdateDelegate();
        // [DllImport("../build/lib/libSupergoonEngine")]
        // public static extern void geContextSetDrawFunc(DrawUpdateDelegate drawFunc);
        [DllImport("../build/lib/libSupergoonEngine")]
        public static extern void DrawDebugRect(ref Rect rect);
    }
    public static class Components
    {
        public static class TagComponent
        {
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool gnTagComponentAddTag(IntPtr component, string tag);
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool gnTagComponentHasTag(IntPtr component, string tag);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gnTagComponentNew();

        }
        public static class ScriptComponent
        {
            public delegate void ScriptComponentDelegate();
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gnScriptComponentNew();
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern int gnScriptComponentSetUpdateFunc(IntPtr scriptComponent, ScriptComponentDelegate func);

        }
        public static class LocationComponent
        {
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern int gnLocationComponentGetX(IntPtr component);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern int gnLocationComponentGetY(IntPtr component);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void gnLocationComponentSetX(IntPtr component, int x);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void gnLocationComponentSetY(IntPtr component, int y);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void gnLocationComponentSetXY(IntPtr component, int x, int y);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr gnLocationComponentNew();
        }
        public static class DrawComponent
        {

            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr geDrawComponentNew(IntPtr locationComponent);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void geDrawComponentDraw(IntPtr component);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void geDrawComponentSetSize(IntPtr component, int w, int h);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void geDrawComponentSetHeight(IntPtr component, int h);
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void geDrawComponentSetWidth(IntPtr component, int w);
        }
        public static class KeyboardComponent
        {
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern IntPtr KeyboardComponentNew();
            [DllImport("../build/lib/libSupergoonEngine")]
            public static extern void SetControllerMapButton(IntPtr keyboardComponent, int buttonToSet, int sdlScancode);
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool CheckIfButtonReleased(IntPtr keyboardComponent, int buttonToCheck);
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool CheckIfButtonPressed(IntPtr keyboardComponent, int buttonToCheck);
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool CheckIfButtonHeld(IntPtr keyboardComponent, int buttonToCheck);
            [DllImport("../build/lib/libSupergoonEngine")]
            [return: MarshalAs(UnmanagedType.I1)]
            public static extern bool CheckIfButtonDown(IntPtr keyboardComponent, int buttonToCheck);

        }
    }
}
