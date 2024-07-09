
type t

external cpp_mk : unit -> t = "cpp_mk_key160"
let mk ()      = cpp_mk ()
external cpp_len : t -> int = "cpp_len_key160"
let len k      = cpp_len k
external cpp_to_bytes : t -> string = "cpp_to_bytes_key160"
let to_bytes k = cpp_to_bytes k
external cpp_to_hex : t -> string = "cpp_to_hex_key160"
let to_hex k   = cpp_to_hex k
external cpp_from_hex : string -> t = "cpp_from_hex_key160"
let from_hex s = cpp_from_hex s
