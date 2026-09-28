type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

(* AES-256-GCM with a 96-bit nonce and a 128-bit tag; the layout is
   ciphertext || tag, compatible with the Rust 'aes-gcm' crate.
   The nonce must be unique per key.

   encrypt nonce key aad n buf dlen
     encrypts the first dlen bytes of buf in-place (buffer size n, must be >= dlen + 16),
     authenticating aad as well; returns (dlen + 16, buf)
   decrypt nonce key aad n buf
     decrypts and verifies the n bytes of ciphertext || tag in buf in-place;
     returns (n - 16, buf), or (-1, buf) if authentication failed:
     buf is zeroed and must not be used *)
val encrypt : Key96.t -> Key256.t -> string -> int -> b -> int -> int*b
val decrypt : Key96.t -> Key256.t -> string -> int -> b -> int*b
