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
    public static class Log
    {
        public static void Debug(string log) => Api.Debug.LogDebug(log);
        public static void Info(string log) => Api.Debug.LogInfo(log);
        public static void Warn(string log) => Api.Debug.LogWarn(log);
        public static void Error(string log) => Api.Debug.LogError(log);

    }


}