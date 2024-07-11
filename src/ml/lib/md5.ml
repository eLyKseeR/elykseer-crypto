
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

external string : string -> string = "cpp_string_md5"
external buffer : b -> string = "cpp_buffer_md5"
