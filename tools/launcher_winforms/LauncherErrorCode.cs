using System;

namespace LauncherWinForms
{
    public enum LauncherErrorCode
    {
        E001,
        E002,
        E003,
        E004,
        E005,
        E006,
        E007,
        E008,
        E009
    }

    public class LauncherException : Exception
    {
        public LauncherErrorCode Code { get; private set; }

        public LauncherException(LauncherErrorCode code, string message) : base(message)
        {
            Code = code;
        }

        public LauncherException(LauncherErrorCode code, string message, Exception inner) : base(message, inner)
        {
            Code = code;
        }
    }
}
