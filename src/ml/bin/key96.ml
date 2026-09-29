open Elykseer_crypto

let () = Key96.mk () |> Key96.to_hex |> print_endline;
         Key96.from_hex "123456789012345678901234" |> Key96.to_hex |> print_endline;

