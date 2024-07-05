
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

module Sha256 = struct
external string : string -> string -> string = "cpp_string_hmac_sha256"
external buffer : string -> b -> string = "cpp_buffer_hmac_sha256"
end

module Md5 = struct
external string : string -> string -> string = "cpp_string_hmac_md5"
external buffer : string -> b -> string = "cpp_buffer_hmac_md5"
end