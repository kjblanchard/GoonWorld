namespace GoonEngine;
public static class Debug
{
    public enum LogLevel
    {
        Default,
        Debug,
        Info,
        Warn,
        Error,
    }

    public static LogLevel Level { get; set; }
    public static void DebugMessage(string log) => Api.Debug.LogDebug(log);
    public static void InfoMessage(string log) => Api.Debug.LogInfo(log);
    public static void WarnMessage(string log) => Api.Debug.LogWarn(log);
    public static void ErrorMessage(string log) => Api.Debug.LogError(log);
}