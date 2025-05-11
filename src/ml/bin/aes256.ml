open Elykseer_crypto
open Mlcpp_cstdio
open Mlcpp_filesystem

let k = Key256.from_hex "1234567890123456789012345678901234567890123456789012345678901234"

let iv = Key128.from_hex "12345678901234567890123456789012"

let read_file fn =
  Cstdio.File.fopen fn "r" |> function
  | Ok fptr -> begin
    let sz = Filesystem.Path.file_size (Filesystem.Path.from_string fn) in
    let buf = Cstdio.File.Buffer.create (sz + 16) in (* add extra space for alignment *)
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
      let (cnt',buf') = Aes256.encrypt iv k cnt buf in
      write_file cnt' buf' "./test.crypt" |> ignore

let decrypt () =
  read_file "./test.crypt" |> function
  | Error code -> Printf.printf "Error: %d\n" code |> ignore
  | Ok (cnt,buf) ->
      let (cnt',buf') = Aes256.decrypt iv k cnt buf in
      write_file cnt' buf' "./test.plain" |> ignore

let () =
  Global.initialize () |> ignore;
  encrypt () ; decrypt ();
  Global.cleanup () |> ignore
