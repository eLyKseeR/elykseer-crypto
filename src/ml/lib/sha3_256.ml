
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

external string : string -> string = "cpp_string_sha3_256"
external file   : string -> string = "cpp_file_sha3_256"
external buffer : b -> string = "cpp_buffer_sha3_256"
