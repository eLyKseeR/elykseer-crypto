
type b = Mlcpp_cstdio.Cstdio.File.Buffer.ta

module type HmacBase = sig
    val string : string -> string -> string
    val buffer : string -> b -> string
end
    
module Sha256 : HmacBase
module Md5 : HmacBase
