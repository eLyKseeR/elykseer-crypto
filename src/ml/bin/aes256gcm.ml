open Elykseer_crypto
open Mlcpp_cstdio
open Mlcpp_filesystem

let k = Key256.from_hex "1234567890123456789012345678901234567890123456789012345678901234"

(* same key and nonce as the encryptGCM/decryptGCM tools in src/ml/README.in_c *)
let nonce = Key96.from_hex "123456789012345678901234"

(* the C tools authenticate no additional data *)
let aad = ""

let read_file fn =
  Cstdio.File.fopen fn "r" |> function
  | Ok fptr -> begin
    let sz = Filesystem.Path.file_size (Filesystem.Path.from_string fn) in
    let buf = Cstdio.File.Buffer.create (sz + 16) in (* extra space for the 16-byte tag *)
    Cstdio.File.fread buf sz fptr |> function
     | Ok cnt ->
       Ok (cnt, buf)
     | Error (errno,errstr) -> 
       Printf.printf "no:%d err:%s\n" errno errstr ; Error (-98)
    end
  | Error _ -> Error (-99)

let write_file sz buf fn =
  Cstdio.File.fopen fn "w" |> function
  | Ok fptr -> begin
    let sz' = min sz (Cstdio.File.Buffer.size buf) in
    Cstdio.File.fwrite buf sz' fptr |> function
     | Ok cnt ->
       Cstdio.File.fclose fptr |> ignore;
       Ok cnt
     | Error (errno,errstr) -> 
       Printf.printf "no:%d err:%s\n" errno errstr ; Error (-96)
    end
  | Error _ -> Error (-97)


let encrypt () =
  read_file "./test.txt" |> function
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (cnt,buf) ->
      let (cnt',buf') = Aes256gcm.encrypt nonce k aad (Cstdio.File.Buffer.size buf) buf cnt in
      write_file cnt' buf' "./test.gcm" |> ignore

let decrypt () =
  read_file "./test.gcm" |> function
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (cnt,buf) ->
      Aes256gcm.decrypt nonce k aad cnt buf |> function
      | (-1, _) -> print_endline "Error: authentication failed"
      | (cnt',buf') -> write_file cnt' buf' "./test.plain.gcm" |> ignore

let () =
  Global.initialize () |> ignore;
  encrypt () ; decrypt ();
  Global.cleanup () |> ignore
