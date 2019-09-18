using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class AesDecrypt
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_AesDecrypt(IntPtr k, IntPtr iv);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int proc_AesDecrypt(IntPtr cl, int inlen, [MarshalAs(UnmanagedType.LPStr)] string inoutbuf);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int fin_AesDecrypt(IntPtr cl, int inlen, [MarshalAs(UnmanagedType.LPStr)] string outbuf);

        private IntPtr cptr { get; set; } = IntPtr.Zero;

        public AesDecrypt(Key256 k, Key128 iv)
        {
            cptr = mk_AesDecrypt(k.cptr, iv.cptr);
        }

        int process(int inlen, string inoutbuf)
        {
            return proc_AesDecrypt(cptr, inlen, inoutbuf);;
        }

        int finish(int inlen, string outbuf)
        {
            return fin_AesDecrypt(cptr, inlen, outbuf);;
        }
    }
}
