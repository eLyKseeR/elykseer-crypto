with import <nixpkgs> {};

stdenv.mkDerivation rec {
    name = "env";

    #src = ./.;

    # Customizable development requirements
    nativeBuildInputs = [
        cmake
        ninja
        ccache
        git
        gnused
        gettext
        texinfo
        swig
        #gcc
        llvmPackages_19.openmp
        lldb_19
        clang_19
        opam m4
        #global
        cppcheck
        #busybox
        perl538Packages.DigestSHA3
        pkg-config
        autoconf automake libtool
        libassuan libgpg-error
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
      export CC=clang
      export CXX=clang++
    '';

}
