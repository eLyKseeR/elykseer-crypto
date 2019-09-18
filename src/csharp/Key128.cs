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
        static extern string bytes_Key128(IntPtr cl);
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

        public string bytes()
        {
            return bytes_Key128(cptr);
        }

        public string tohex()
        {
            return tohex_Key128(cptr);
        }
    }
}
