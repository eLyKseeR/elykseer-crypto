open Mlcpp_cstdio

type b = Cstdio.File.Buffer.ta

external encrypt : Key96.t -> Key256.t -> string -> int -> b -> int -> int*b = "cpp_encrypt_aes256_gcm_bytecode" "cpp_encrypt_aes256_gcm"
external decrypt : Key96.t -> Key256.t -> string -> int -> b -> int*b = "cpp_decrypt_aes256_gcm"
