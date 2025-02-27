FROM mstorsjo/llvm-mingw:latest
# see: https://github.com/mstorsjo/llvm-mingw

# this prepares the Docker image for 
# cross-compiling eLyKseeR to Windows

# docker build -t xcompile .

# docker run -it --rm --user=$(id -u):$(id -g) -v .:/work xcompile

# Debian based
RUN apt update && apt upgrade -y

# >>> adapt the uid and gid to the file owner <<<
RUN addgroup --gid 1000 lxr && adduser --disabled-password --uid 501 --gid 1000 lxr

RUN mkdir -vp /home/lxr/.local && chown -v lxr:lxr /home/lxr/.local

# everything below will run as user "lxr"
USER lxr 

WORKDIR /home/lxr

# download cmake from https://cmake.org/download/
COPY ext/cmake-3.31.5.tar.gz cmake-3.31.5.tar.gz

RUN tar xzf cmake-3.31.5.tar.gz

WORKDIR cmake-3.31.5/build

RUN cmake -DCMAKE_USE_OPENSSL=OFF -DCMAKE_INSTALL_PREFIX=${HOME}/.local -GNinja ..

RUN ninja && ninja install

WORKDIR /home/lxr

# download ninja from https://github.com/ninja-build/ninja/releases
COPY ext/ninja-1.12.1.tar.gz ninja-1.12.1.tar.gz

RUN tar xzf ninja-1.12.1.tar.gz

WORKDIR ninja-1.12.1/build

RUN cmake -GNinja -DCMAKE_INSTALL_PREFIX=${HOME}/.local ..

RUN ninja && ninja install

WORKDIR /work

ENTRYPOINT bash