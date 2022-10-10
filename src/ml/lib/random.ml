
type t

external setup : unit -> t = "cpp_mk_rng"
external random32 : t -> int = "cpp_r32_rng"
external r32_below : t -> int -> int = "cpp_r32_below_rng"
let r32_range r lo up =
  r32_below r (up - lo) + lo
let f32_range r lo up =
  float_of_int (random32 r) *. (up -. lo) /. (1e32 -. 1.0) +. lo
let with_rng f = let r = setup () in f r