using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class AesDecrypt
    {
        [DllImport("elykseer-crypto.dll", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_AesDecrypt(IntPtr k, IntPtr iv);

        private IntPtr cptr { get; set; } = IntPtr.Zero;

        public AesDecrypt(Key256 k, Key128 iv)
        {
            cptr = mk_AesDecrypt(k.cptr, iv.cptr);
        }
    }
}
