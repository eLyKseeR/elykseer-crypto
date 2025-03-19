
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

(* pass in the initialisation vector (128 bits)
   and the key (256 bits) to de/encrypt buffer "b"
   in-place which is returned back
*)
val encrypt : Key128.t -> Key256.t -> int -> b -> int*b
val decrypt : Key128.t -> Key256.t -> int -> b -> int*b
