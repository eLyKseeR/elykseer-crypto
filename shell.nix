with import <nixpkgs> {};

stdenv.mkDerivation rec {
    name = "env";

    #src = ./.;

    # Customizable development requirements
    nativeBuildInputs = [
        cmake
        git
        gnused
        pandoc
        html2text
        swig
        gcc
        clang
        opam m4
        #global
        cppcheck
        #busybox
        perl534Packages.DigestSHA3
        pkg-config
        autoconf automake libtool
        libassuan libgpgerror
        gnupg
    ];

    buildInputs = [
        openssl
        zlib
        boost
    ];

    shellHook = ''
      echo 'eLyKseeR nixified environment'
      export SED=sed
    '';

}

