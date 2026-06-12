using System;
using System.Runtime.InteropServices;

namespace Bolun.Phone.OS
{
    public static class BolunNative
    {
        [DllImport("bolun", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr bolun_app_count();

        [DllImport("bolun", CallingConvention = CallingConvention.Cdecl)]
        public static extern int bolun_service_start_all();
    }
}
