open Mlcpp_cstdio

type b = Cstdio.File.Buffer.ta

external encrypt : Key128.t -> Key256.t -> int -> b -> int*b = "cpp_encrypt_aes256"
external decrypt : Key128.t -> Key256.t -> int -> b -> int*b = "cpp_decrypt_aes256"
