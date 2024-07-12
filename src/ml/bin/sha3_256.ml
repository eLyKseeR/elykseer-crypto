open Elykseer_crypto
open Mlcpp_cstdio
open Mlcpp_filesystem

let msg = "1234567890123456789012345678901234567890123456789012345678901234"

let read_file fn =
  Cstdio.File.fopen fn "rb" |> function
  | Ok fptr -> begin
    let sz = Filesystem.Path.file_size (Filesystem.Path.from_string fn) in
    let buf = Cstdio.File.Buffer.create sz in
    Cstdio.File.fread buf sz fptr |> function
     | Ok cnt ->
       Ok (cnt, buf)
     | Error (errno,errstr) -> 
       Printf.printf "no:%d err:%s\n" errno errstr ; Error (-98)
    end
  | Error _ -> Error (-99)

let sha3file () =
  read_file "/bin/sh" |> function
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (_cnt,buf) ->
      Sha3_256.buffer buf |> Printf.printf "sha3_256 of file: %s\n"

let sha3msg () =
  Sha3_256.string msg |> Printf.printf "sha3_256 of msg: %s\n"

let sha3path () =
  Sha3_256.file "/bin/sh" |> Printf.printf "sha3_256 of path: %s\n"

let () =
  sha3file () ; sha3msg () ; sha3path ()