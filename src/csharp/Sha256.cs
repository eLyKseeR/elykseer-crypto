using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Sha256
    {
        [DllImport("elykseer-crypto", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Sha256();

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Sha256()
        {
            cptr = mk_Sha256();
        }
    }
}
