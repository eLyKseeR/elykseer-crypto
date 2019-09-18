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
        static extern string bytes_Key256(IntPtr cl);
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern string tohex_Key256(IntPtr cl);

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Key256()
        {
            cptr = mk_Key256();
        }

        int length()
        {
            return len_Key256(cptr);
        }

        string bytes()
        {
            return bytes_Key256(cptr);
        }

        string tohex()
        {
            return tohex_Key256(cptr);
        }
    }
}
