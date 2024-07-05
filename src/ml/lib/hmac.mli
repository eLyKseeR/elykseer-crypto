
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

module Sha256 : sig
val string : string -> string -> string
val buffer : string -> b -> string
end

module Md5 : sig
val string : string -> string -> string
val buffer : string -> b -> string
end