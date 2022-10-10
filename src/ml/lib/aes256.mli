
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

(** the buffer's maximum size is Aes::datasz 
    which is currently set to 4096 bytes *)

val datasz : unit -> int

module Encrypt :
sig
  type t
  val mk       : Key256.t -> Key128.t -> t
  val process  : t -> int -> b -> int
  val finish   : t -> int -> b -> int
end

module Decrypt :
sig
  type t
  val mk       : Key256.t -> Key128.t -> t
  val process  : t -> int -> b -> int
  val finish   : t -> int -> b -> int
end
