using System;

namespace elykseercryptoapp
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            var h = lxr.Md5.hash("some text");
            Console.WriteLine(h.tohex());
        }
    }
}
