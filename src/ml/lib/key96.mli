
type t

val mk       : unit -> t
val len      : t -> int
val to_bytes : t -> string
val to_hex   : t -> string
val from_hex : string -> t
