using System;

namespace lxr.demo
{
    class Demo
    {
        public static void Main(string[] args)
        {
            // Md5
            const string m = "Hello World!";
            Console.WriteLine(m);
            for (var i = 0; i < 100; i++)
            {
                var h = lxr.Md5.hash(m);
                Console.WriteLine(h.tohex());
            }
            // Key128
            var k1 = new Key128();
            Console.WriteLine("key 128 bit with length = {0} and hex representation = {1}({2})", k1.length(), k1.tohex(), k1.bytes().Length);
            // Key256
            var k2 = new Key256();
            Console.WriteLine("key 256 bit with length = {0} and hex representation = {1}({2})", k2.length(), k2.tohex(), k2.bytes().Length);
        }
    }
}
