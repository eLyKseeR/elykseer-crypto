using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class AesEncrypt
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_AesEncrypt(IntPtr k, IntPtr iv);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int proc_AesEncrypt(IntPtr cl, int inlen, [MarshalAs(UnmanagedType.LPStr)] string inoutbuf);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int fin_AesEncrypt(IntPtr cl, int inlen, [MarshalAs(UnmanagedType.LPStr)] string outbuf);

        private IntPtr cptr { get; set; } = IntPtr.Zero;

        public AesEncrypt(Key256 k, Key128 iv)
        {
            cptr = mk_AesEncrypt(k.cptr, iv.cptr);
        }

        int process(int inlen, string inoutbuf)
        {
            return proc_AesEncrypt(cptr, inlen, inoutbuf);;
        }

        int finish(int inlen, string outbuf)
        {
            return fin_AesEncrypt(cptr, inlen, outbuf);;
        }
    }
}
