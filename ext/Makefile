
.PHONY: all unit clean debug release xWin64

all: release

export BASEP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
export CCACHE_BASEDIR := $(BASEP)
export CCACHE_DIR = $(CCACHE_BASEDIR)/_ccache/

# host system
ifeq ($(OS),Windows_NT)
	export uname_s := Windows
	export uname_m := $(MSYSTEM_CARCH)
	export CMAKE_GENERATOR := MSYS Makefiles
else
	export uname_s := $(shell uname -s)
	export uname_m := $(shell uname -m)
	export CMAKE_GENERATOR := Unix Makefiles
endif

ifndef (HOST_TARGETDIR)
	export HOST_TARGETDIR := $(BASEP)/$(uname_s)_$(uname_m)
endif
ifndef (WIN64_TARGETDIR)
	export WIN64_TARGETDIR := $(BASEP)/xWindows_x86_64
endif

clean:
	ls -l $(HOST_TARGETDIR)

release:
	export TARGETDIR=$(HOST_TARGETDIR); \
	export HOST_SELECTOR=""; \
	export CMAKE_BUILD_TYPE="Release"; \
	export PKG_CONFIG_PATH="${HOST_TARGETDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}"; \
	$(MAKE) make_exts

debug:
	export TARGETDIR=$(HOST_TARGETDIR); \
	export HOST_SELECTOR=""; \
	export CMAKE_BUILD_TYPE="Debug"; \
	export PKG_CONFIG_PATH="${HOST_TARGETDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}"; \
	$(MAKE) make_exts

xWin64: check_cross_compilers
	export CROSS_COMPILATION=1; \
	export TARGETDIR=$(WIN64_TARGETDIR); \
	export HOST_SELECTOR="--host=x86_64-w64-mingw32"; \
	export CMAKE_BUILD_TYPE="Release"; \
	export PKG_CONFIG_PATH="$(WIN64_TARGETDIR)/lib/pkgconfig"; \
	$(MAKE) make_exts

makedirs: $(CCACHE_DIR) $(HOST_TARGETDIR) $(WIN64_TARGETDIR)

$(CCACHE_DIR) $(HOST_TARGETDIR) $(WIN64_TARGETDIR):
	mkdir -vp $@

check_cross_compilers: check_cc check_cxx check_ar check_ranlib
check_ar:
	@[ -n "$(AR)" ] && type $(AR) 2> /dev/null

check_cc:
	@[ -n "$(CC)" ] && type $(CC) 2> /dev/null

check_cxx:
	@[ -n "$(CXX)" ] && type $(CXX) 2> /dev/null

check_ranlib:
	@[ -n "$(RANLIB)" ] && type $(RANLIB) 2> /dev/null


make_exts: makedirs make_libgpg_error make_libassuan make_gpgme make_cryptopp make_openssl
	echo "all done."

fset_libgpg_error := $(TARGETDIR)/include/gpg-error.h $(TARGETDIR)/lib/libgpg-error.a
make_libgpg_error: $(fset_libgpg_error)
$(fset_libgpg_error):
	echo "making libgpg-error"
	cd ./libgpg-error; \
	./autogen.sh; \
	./configure --enable-static --disable-shared  --disable-doc --with-pic --prefix=${TARGETDIR} --enable-install-gpg-error-config ${HOST_SELECTOR}; \
	$(MAKE); \
	$(MAKE) install

fset_libassuan := $(TARGETDIR)/include/assuan.h $(TARGETDIR)/lib/libassuan.a
make_libassuan: $(fset_libassuan)
$(fset_libassuan):
	echo "making libassuan"
	cd ./libassuan; \
	./autogen.sh; \
	./configure --enable-static --disable-shared  --disable-doc --with-pic --prefix=${TARGETDIR} --with-libgpg-error-prefix=${TARGETDIR} ${HOST_SELECTOR}; \
	$(MAKE); \
	$(MAKE) install

fset_gpgme := $(TARGETDIR)/include/gpgme.h $(TARGETDIR)/lib/libgpgme.a
make_gpgme: $(fset_gpgme)
$(fset_gpgme):
	echo "making gpgme"
	cd ./gpgme; \
	./autogen.sh; \
	./configure --enable-static --disable-shared --disable-gpgsm-test --disable-g13-test --disable-gpg-test --disable-gpgconf-test --disable-doc --with-pic --with-libassuan-prefix=${TARGETDIR} --with-libgpg-error-prefix=${TARGETDIR} --prefix=${TARGETDIR} ${HOST_SELECTOR}; \
	$(MAKE); \
	$(MAKE) install

fset_cryptopp:= $(TARGETDIR)/include/cryptopp/config.h $(TARGETDIR)/lib/libcryptopp.a
make_cryptopp: $(fset_cryptopp)
$(fset_cryptopp):
	echo "making cryptopp"
	cd ./cryptopp; \
	$(MAKE) -f GNUmakefile clean; \
	$(MAKE) -f GNUmakefile -j2; \
	export PREFIX=${TARGETDIR}; \
	$(MAKE) -f GNUmakefile install

fset_openssl:= $(TARGETDIR)/include/openssl/opensslconf.h $(TARGETDIR)/lib/libcrypto.a $(TARGETDIR)/lib/libssl.a
make_openssl: $(fset_openssl)
$(fset_openssl):
	echo "making openssl"
	cd ./openssl.git; \
	./Configure no-docs no-dso no-engine no-filenames no-http no-legacy no-module no-ocsp no-shared no-tests threads no-ui-console no-uplink --prefix=${TARGETDIR} --openssldir=${TARGETDIR} --libdir=lib; \
	$(MAKE); \
 	$(MAKE) install

