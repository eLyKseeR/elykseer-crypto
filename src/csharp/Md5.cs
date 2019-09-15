using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Md5
    {
        [DllImport("elykseer-crypto.dll", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Md5();

        private IntPtr cptr { get; set; } = IntPtr.Zero;

        public Md5()
        {
            cptr = mk_Md5();
        }
    }
}
