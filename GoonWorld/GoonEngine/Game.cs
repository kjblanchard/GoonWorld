using System.Runtime.InteropServices;
namespace GoonEngine;

public class Game
{
    public ConfigManager _config;
    public Game()
    {
        _config = new ConfigManager();
    }
    ~Game()
    {

    }

    public bool CreateWindow()
    {
        LuaCreateSdlWindow(_config.Config.windowConfig.windowSize.x, _config.Config.windowConfig.windowSize.y);
        return true;
    }

    public bool Initialize()
    {
        return true;
    }


    [DllImport("../build/lib/libSupergoonEngine")]
    private static extern int LuaCreateSdlWindow(uint width, uint height);
}
