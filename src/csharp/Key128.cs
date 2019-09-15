using System;
using System.Runtime.InteropServices;

namespace lxr
{
    public class Key128
    {
        [DllImport("elykseer-crypto.dll", CharSet = CharSet.Ansi)]
        static extern IntPtr mk_Key128();

        public IntPtr cptr { get; set; } = IntPtr.Zero;

        public Key128()
        {
            cptr = mk_Key128();
        }
    }
}
