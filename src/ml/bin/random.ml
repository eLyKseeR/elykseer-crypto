open Elykseer_crypto

let t1 = Random.random32 () |> string_of_int |> print_endline

exception Killed of int * string
let t2 =
  for i = 1 to 1000000 do
    let r = Random.random32 () in
    if r < 0 && r > (1 lsl 32)-1 then
      raise (Killed (i, "out of 32 bit unsigned integer range"))
  done

let t3 =
  for i = 1 to 1000000 do
    let lo = 102023
    and hi = 9999283 in
    let r = Random.r32_range lo hi in
    if r < lo && r >= hi then
      raise (Killed (i, "out of restricted unsigned integer range"))
  done

let t4 =
  for i = 1 to 1000000 do
    let lo = 102023.0
    and hi = 9999283.0 in
    let r = Random.f32_range lo hi in
    if r < lo && r >= hi then
      raise (Killed (i, "out of restricted unsigned integer range"))
  done

let () = t1 ; t2 ; t3 ; t4
