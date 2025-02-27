
type t

external random32 : unit -> int = "cpp_r32_rng"
external r32_below : int -> int = "cpp_r32_below_rng"
let r32_range lo up =
  r32_below (up - lo) + lo
let f32_range lo up =
  float_of_int (random32 ()) *. (up -. lo) /. (1e32 -. 1.0) +. lo