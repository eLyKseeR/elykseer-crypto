open Elykseer_crypto
open Mlcpp_cstdio

let k = Key256.from_hex "1234567890123456789012345678901234567890123456789012345678901234"

let iv = Key128.from_hex "12345678901234567890123456789012"

let read_file fn =
  Cstdio.File.fopen fn "r" |> function
  | Ok fptr -> begin
    let sz = (4*1024) in
    let buf = Cstdio.File.Buffer.create sz in
    Cstdio.File.fread buf sz fptr |> function
     | Ok cnt ->
       (* Printf.printf "  read:%s\n" (Cstdio.File.Buffer.to_string buf) ; *)
       Ok (cnt, buf)
     | Error (errno,errstr) -> 
       Printf.printf "no:%d err:%s\n" errno errstr ; Error (-98)
    end
  | Error _ -> Error (-99)

let write_file sz buf fn =
  Cstdio.File.fopen fn "w" |> function
  | Ok fptr -> begin
    Cstdio.File.fwrite buf sz fptr |> function
     | Ok cnt ->
       Printf.printf "  wrote:%d\n" cnt ;
       Cstdio.File.fclose fptr |> ignore;
       Ok cnt
     | Error (errno,errstr) -> 
       Printf.printf "no:%d err:%s\n" errno errstr ; Error (-96)
    end
  | Error _ -> Error (-97)


let encrypt () =
  let aes = Aes256.Encrypt.mk k iv in
  match read_file "/tmp/test.txt" with
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (cnt,buf) ->
      let enc1 = Aes256.Encrypt.process aes cnt buf in
      let enc2 = Aes256.Encrypt.finish aes enc1 buf in
      Printf.printf "Encrypt: %d + %d\n" enc1 enc2 |> ignore;
      write_file (enc1 + enc2) buf "/tmp/test.crypt" |> ignore

let decrypt () =
  let aes = Aes256.Decrypt.mk k iv in
  match read_file "/tmp/test.crypt" with
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (cnt,buf) ->
      let enc1 = Aes256.Decrypt.process aes cnt buf in
      let enc2 = Aes256.Decrypt.finish aes enc1 buf in
      Printf.printf "Decrypt: %d + %d\n" enc1 enc2 |> ignore;
      write_file (enc1 + enc2) buf "/tmp/test.plain" |> ignore

let () =
  encrypt () ; decrypt ()
