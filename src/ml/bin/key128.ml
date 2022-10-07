open Elykseer_crypto

let () = Key128.mk () |> Key128.to_hex |> print_endline;
         Key128.from_hex "12345678901234567890123456789012" |> Key128.to_hex |> print_endline;

