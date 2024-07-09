open Elykseer_crypto

let key = "Jefe"
let m = "what do ya want for nothing?"

let t1 =
  Hmac.Md5.string key m |> print_endline

let t2 =
  Hmac.Sha256.string key m |> print_endline

let t3 =
  Hmac.Sha1.string "test" "test" |> print_endline

let () = t1 ; t2 ; t3