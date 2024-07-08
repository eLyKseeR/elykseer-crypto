open Elykseer_crypto

let msg = "hello world."
let b64 = "aGVsbG8gd29ybGQu"

let t1 =
  Base64.encode msg |> print_string; print_string " = ";
  print_endline b64

  let t2 =
    Base64.decode b64 |> print_string; print_string " = ";
    print_endline msg
  
let () = t1 ; t2