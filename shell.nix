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
        mono
        busybox
    ];

    buildInputs = [
        openssl
        zlib
        boost
    ];

}

