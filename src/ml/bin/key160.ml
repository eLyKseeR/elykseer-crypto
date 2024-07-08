open Elykseer_crypto

let () = Key160.mk () |> Key160.to_hex |> print_endline;
         Key160.from_hex "1234567890123456789012345678901234567890" |> Key160.to_hex |> print_endline;

