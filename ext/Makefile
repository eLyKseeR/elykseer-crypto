
.PHONY: all unit clean debug release

all: release

export BASEP := $(shell pwd)
export CCACHE_BASEDIR := $(BASEP)
export CCACHE_DIR = $(CCACHE_BASEDIR)/_ccache/
#export CMAKE_BUILD_TYPE = $(BUILD_TYPE)


ifeq ($(OS),Windows_NT)
	uname_s := Windows
	uname_m := $(ARCHITECTURE)
	export CMAKE_GENERATOR := MinGW Makefiles
else
	uname_s := $(shell uname -s)
	uname_m := $(shell uname -m)
	export CMAKE_GENERATOR := Unix Makefiles
endif

export TARGETDIR := $(BASEP)/$(uname_s)_$(uname_m)

clean:
	ls -l $(TARGETDIR)

release: makedirs
	export CMAKE_BUILD_TYPE="Release"; \
	export PKG_CONFIG_PATH=${TARGETDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}; \
	$(MAKE) make_exts

debug: makedirs
	export CMAKE_BUILD_TYPE="Debug"; \
	export PKG_CONFIG_PATH=${TARGETDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}; \
	$(MAKE) make_exts

makedirs: $(CCACHE_DIR) $(TARGETDIR)

$(CCACHE_DIR) $(TARGETDIR):
	mkdir -vp $@

make_exts: make_gitalk make_libgpg_error make_libassuan make_gpgme make_cryptopp
	echo "all done."

fset_gitalk := $(TARGETDIR)/bin/html2text
make_gitalk: $(fset_gitalk)
$(fset_gitalk):
	echo "making gitalk"
	cd ./gitalk; \
	git submodule update --init; \
	cd ext/html2text.git; \
	PREFIX=${TARGETDIR} ./configure; \
	$(MAKE); \
	$(MAKE) install

fset_libgpg_error := $(TARGETDIR)/include/gpg-error.h $(TARGETDIR)/lib/libgpg-error.a
make_libgpg_error: $(fset_libgpg_error)
$(fset_libgpg_error):
	echo "making libgpg-error"
	cd ./libgpg-error; \
	./autogen.sh; \
	./configure --enable-static --disable-shared  --disable-doc --with-pic --prefix=${TARGETDIR} --enable-install-gpg-error-config; \
	$(MAKE); \
	$(MAKE) install

fset_libassuan := $(TARGETDIR)/include/assuan.h $(TARGETDIR)/lib/libassuan.a
make_libassuan: $(fset_libassuan)
$(fset_libassuan):
	echo "making libassuan"
	cd ./libassuan; \
	./autogen.sh; \
	./configure --enable-static --disable-shared  --disable-doc --with-pic --prefix=${TARGETDIR} --with-libgpg-error-prefix=${TARGETDIR}; \
	$(MAKE); \
	$(MAKE) install

fset_gpgme := $(TARGETDIR)/include/gpgme.h $(TARGETDIR)/lib/libgpgme.a
make_gpgme: $(fset_gpgme)
$(fset_gpgme):
	echo "making gpgme"
	cd ./gpgme; \
	./autogen.sh; \
	./configure --enable-static --disable-shared --enable-languages=cpp --disable-gpgsm-test --disable-g13-test --disable-gpg-test --disable-gpgconf-test --disable-doc --with-pic --with-libassuan-prefix=${TARGETDIR} --with-libgpg-error-prefix=${TARGETDIR} --prefix=${TARGETDIR}; \
	$(MAKE); \
	$(MAKE) install

fset_cryptopp:= $(TARGETDIR)/include/cryptopp/config.h $(TARGETDIR)/lib/libcryptopp.a
make_cryptopp: $(fset_cryptopp)
$(fset_cryptopp):
	echo "making cryptopp"
	cd ./cryptopp; \
	$(MAKE) -f GNUmakefile -j2; \
	export PREFIX=${TARGETDIR}; \
	$(MAKE) -f GNUmakefile install


