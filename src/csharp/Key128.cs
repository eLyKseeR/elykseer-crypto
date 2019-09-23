using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Key128
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Key128();
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int len_Key128(IntPtr cl);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        static extern byte[] bytes_Key128(IntPtr cl);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        static extern string tohex_Key128(IntPtr cl);

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Key128()
        {
            cptr = mk_Key128();
        }
        public Key128(IntPtr ptr)
        {
            cptr = ptr;
        }

        public int length()
        {
            return len_Key128(cptr);
        }

        public byte[] bytes()
        {
            byte[] bs = bytes_Key128(cptr);
            int l = 128 / 8;
            var newbs = new byte[l];
            for (int i=0; i<l; i++) { newbs[i] = bs[i]; }
            return newbs;
        }

        public string tohex()
        {
            return tohex_Key128(cptr);
        }
    }
}
