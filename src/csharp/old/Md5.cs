using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Md5
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern IntPtr hash_Md5(int len, [MarshalAs(UnmanagedType.LPStr)] string m);

        private Md5()
        {
        }

        public static Key128 hash(string msg)
        {
            IntPtr k = hash_Md5(msg.Length, msg);
            return new Key128(k);
        }
    }
}
