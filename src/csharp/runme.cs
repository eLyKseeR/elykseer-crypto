using System;
using System.Text;

public class runme {
    public static byte[] EncodeToBytes(string str, int len)
    {
        byte[] bytes = new byte[len * sizeof(char)];
        System.Buffer.BlockCopy(str.ToCharArray(), 0, bytes, 0, bytes.Length);
        return bytes;
    }

    public static string DecodeToString(byte[] bytes, int start, int len)
    {
        char[] chars = new char[len / sizeof(char)];
        System.Buffer.BlockCopy(bytes, start, chars, 0, len);
        return new string(chars);
    }

    static void Main() {
         var m = "HELLO WORLD";
         var md5 = lxr.Md5.hash_Md5(m.Length, m);
         var hk1 = lxr.Key128.tohex_Key128(md5);
         var k2 = lxr.Key128.fromhex_Key128("361fadf1c712e812d198c4cab5712a79");
		 var hk2 = lxr.Key128.tohex_Key128(k2);
 		 Console.WriteLine("{0} == {1} : {2}", hk1, hk2, hk1==hk2);
         lxr.Key128.release_Key128(md5);
         lxr.Key128.release_Key128(k2);
         Console.WriteLine(lxr.Key256.tohex_Key256(lxr.Key256.mk_Key256()));
         var h2 = lxr.Sha256.filehash_Sha256("libelykseer-crypto-cs.so");
         Console.WriteLine(lxr.Key256.tohex_Key256(h2));

         var k = lxr.Key256.mk_Key256();
         var iv = lxr.Key128.mk_Key128();
         var AesEnc = lxr.Aes.mk_AesEncrypt(k, iv);
         char[] chars = new char[1024];
         string str = new string(chars);
         var tencr = new byte[256];
         for (var i = 0; i < 20; i++) {
            str = "0123456789";
            var proc = lxr.Aes.proc_AesEncrypt(AesEnc, 10, str);
            var bencr = EncodeToBytes(str, proc);
            for (var j = 0; j < bencr.Length; j++) {
               tencr[i*10+j] = bencr[j];
            }
         }
         str = "0123456789";
         var finc = lxr.Aes.fin_AesEncrypt(AesEnc, 10, str);
         if (finc > 0) {
           var finb = EncodeToBytes(str, finc);
           for (var j = 0; j < finb.Length; j++) {
               tencr[20*10+j] = finb[j];
           }
         }

         var AesDec = lxr.Aes.mk_AesDecrypt(k, iv);
         var tdecr = new byte[256];
         str = new string(chars);
         for (var i = 0; i < 20; i++) {
            str = DecodeToString(tencr, i*12, 12);
            var proc = lxr.Aes.proc_AesDecrypt(AesDec, 12, str);
            Console.WriteLine("{0} : {1}", proc, str);
         }

         // cleanup
         lxr.Key128.release_Key128(iv);
		 lxr.Key256.release_Key256(k);

		aMillionKeys ();
	}
	private static void aMillionKeys ()
  {
    for (int i = 0; i < 100000; i++) {
         var k = lxr.Key256.mk_Key256();
         var h = lxr.Key256.tohex_Key256(k);
         Console.WriteLine("{0} ", h);
		 lxr.Key256.release_Key256(k);

		}
		Console.WriteLine("done");
  }

 }
