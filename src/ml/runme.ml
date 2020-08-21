open Lxr_Key128;;
let p = _mk_Key128 C_void in
_tohex_Key128 p |> Swig.get_string |> print_endline;
_release_Key128 p;;

open Lxr_Sha256;;
open Lxr_Key256;;
let h = _filehash_Sha256 (C_string "./elykseerTop") in
_tohex_Key256 h |> Swig.get_string |> print_endline;
_release_Key256 h;;


(*
using System;
using System.Text;

public class runme {
    public static byte[] SubArray(byte[] bytesin, int start, int len)
    {
        byte[] bytes = new byte[len];
        System.Buffer.BlockCopy(bytesin, start, bytes, 0, len);
        return bytes;
    }
    public static byte[] EncodeToBytes(string str)
    {
        return Encoding.UTF8.GetBytes(str);
    }

    public static string DecodeToString(byte[] bytes, int start, int len)
    {
        byte[] chars = new byte[len];
        //char[] chars = new char[len / sizeof(char)];
        System.Buffer.BlockCopy(bytes, start, chars, 0, len);
        //return new string(chars);
        return Encoding.UTF8.GetString(chars);
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
          lxr.Key256.release_Key256(h2);


         byte[] cipher = new byte[2*lxr.Aes.datasz];
         byte[] partialcipher;
         var k = lxr.Key256.mk_Key256();
         var iv = lxr.Key128.mk_Key128();
         var AesEnc = lxr.Aes.mk_AesEncrypt(k, iv);
         uint totenc = 0;
         uint copied = 0;
         byte[] str = EncodeToBytes("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
         for (var i = 0; i < 44; i++) {
            int proc = lxr.Aes.proc_AesEncrypt(AesEnc, (uint)str.Length, str);
            if (proc > 0)
            {
                totenc += (uint)proc;
                Console.WriteLine("encrypted: {0}", proc);
                uint lcopied = lxr.Aes.copy_AesEncrypt(AesEnc, totenc, out partialcipher);
                if (lcopied > 0)
                {
                    System.Buffer.BlockCopy(partialcipher, 0, cipher, (int)copied, (int)lcopied);
                    copied += lcopied;
                    Console.WriteLine("copied: {0}  total: {1}", lcopied, copied);
                }
            } else
            {
                Console.WriteLine("error while encrypting!");
            }
         }
         int fenc = lxr.Aes.fin_AesEncrypt(AesEnc);
         if (fenc > 0) {
           Console.WriteLine("encrypted (fin): {0}", fenc);
           totenc += (uint)fenc;
         }
         lxr.Aes.release_AesEncrypt(AesEnc);

         var AesDec = lxr.Aes.mk_AesDecrypt(k, iv);
         uint totdec = 0;
         for (var i = 0; i < 20; i++) {
            int proc = lxr.Aes.proc_AesDecrypt(AesDec, 12, SubArray(cipher, i * 12, 12));
            if (proc > 0)
            {
                totdec += (uint)proc;
                // Console.WriteLine("decrypted: {0}", proc);
            }
            else
            {
                Console.WriteLine("error while decrypting!");
            }
         }
         int fdec = lxr.Aes.fin_AesDecrypt(AesDec);
         if (fdec > 0) {
            Console.WriteLine("decrypted (fin): {0}", fdec);
            totdec += (uint)fdec;
        }
        Console.WriteLine("encrypted: {0}  decrypted: {1}", totenc, totdec);
        byte[] decrypted = new byte[lxr.Aes.datasz];
        uint restored = lxr.Aes.copy_AesDecrypt(AesDec, totdec, out partialcipher);
        if (restored > 0)
        {
            System.Buffer.BlockCopy(partialcipher, 0, decrypted, 0, (int)restored);
            Console.WriteLine("decrypted: {0} <> {1} <> {2} ..", decrypted[0], decrypted[1], decrypted[2]);
            Console.WriteLine("{0}", DecodeToString(decrypted, 0, (int)restored));
        }
        lxr.Aes.release_AesDecrypt(AesDec);

        // cleanup
        lxr.Key128.release_Key128(iv);
        lxr.Key256.release_Key256(k);

         //aMillionKeys ();
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
 *)
