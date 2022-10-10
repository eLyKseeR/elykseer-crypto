open Elykseer_crypto

let () = Key256.mk () |> Key256.to_hex |> print_endline;
         Key256.from_hex "1234567890123456789012345678901234567890123456789012345678901234" |> Key256.to_hex |> print_endline;

