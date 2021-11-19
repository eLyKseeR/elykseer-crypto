with import <nixpkgs> {};

stdenv.mkDerivation rec {
    name = "env";

    #src = ./.;

    # Customizable development requirements
    nativeBuildInputs = [
        cmake
        git
        pandoc
        html2text
        swig
        gcc
        clang
        opam m4
        #busybox
    ];

    buildInputs = [
        openssl
        zlib
        boost
    ];

}

