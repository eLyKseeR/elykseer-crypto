open Mlcpp_cstdio

type b = Cstdio.File.Buffer.ta

external datasz : unit -> int = "cpp_aes_datasz"

module Encrypt = struct
  type t
  external mk : Key256.t -> Key128.t -> t = "cpp_mk_aes256encrypt"
  external process : t -> int -> b -> int = "cpp_process_aes256encrypt"
  external finish : t -> int -> b -> int = "cpp_finish_aes256encrypt"
end

module Decrypt = struct
  type t
  external mk : Key256.t -> Key128.t -> t = "cpp_mk_aes256decrypt"
  external process : t -> int -> b -> int = "cpp_process_aes256decrypt"
  external finish : t -> int -> b -> int = "cpp_finish_aes256decrypt"
end
