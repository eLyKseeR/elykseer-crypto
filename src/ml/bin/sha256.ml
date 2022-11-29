open Elykseer_crypto
open Mlcpp_cstdio
open Mlcpp_filesystem

let msg = "1234567890123456789012345678901234567890123456789012345678901234"

let read_file fn =
  Cstdio.File.fopen fn "r" |> function
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

let shafile () =
  read_file "/tmp/test.txt" |> function
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (_cnt,buf) ->
      Sha256.buffer buf |> Printf.printf "sha256 of file: %s\n"

let shamsg () =
  Sha256.string msg |> Printf.printf "sha256 of msg: %s\n"

let shapath () =
  Sha256.file "/bin/sh" |> Printf.printf "sha256 of path: %s\n"

let () =
  shafile () ; shamsg () ; shapath ()