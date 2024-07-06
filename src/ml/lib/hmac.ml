
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

module type HmacBase = sig
  val string : string -> string -> string
  val buffer : string -> b -> string
end

module Sha256 : HmacBase = struct
external string : string -> string -> string = "cpp_string_hmac_sha256"
external buffer : string -> b -> string = "cpp_buffer_hmac_sha256"
end

module Md5 : HmacBase = struct
external string : string -> string -> string = "cpp_string_hmac_md5"
external buffer : string -> b -> string = "cpp_buffer_hmac_md5"
end