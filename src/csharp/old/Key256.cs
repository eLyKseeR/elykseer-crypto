using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Key256
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Key256();
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern int len_Key256(IntPtr cl);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern byte[] bytes_Key256(IntPtr cl);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern string tohex_Key256(IntPtr cl);

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Key256()
        {
            cptr = mk_Key256();
        }

        public int length()
        {
            return len_Key256(cptr);
        }

        public byte[] bytes()
        {
            byte[] bs = bytes_Key256(cptr);
            int l = 256 / 8;
            var newbs = new byte[l];
            for (int i = 0; i < l; i++) { newbs[i] = bs[i]; }
            return newbs;
        }

        public string tohex()
        {
            return tohex_Key256(cptr);
        }
    }
}
