using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Key256
    {
        [DllImport("elykseer-crypto.dll", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Key256();

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Key256()
        {
            cptr = mk_Key256();
        }
    }
}
