
type t

val setup     : unit -> t
val random32  : t -> int
val r32_below : t -> int -> int
val r32_range : t -> int -> int -> int
val f32_range : t -> float -> float -> float
val with_rng  : (t -> 'a) -> 'a
