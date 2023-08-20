# flutter-five

### .bashrc
export R5=/opt/riscv
export PATH="$PATH:$R5/bin"
export R5_SYSROOT=$R5/sysroot
export R5_PREFIX=$R5_SYSROOT/usr
export R5_PKG_CONFIG_PATH=$R5_PREFIX/lib/pkgconfig
export R5_HOST=riscv64-unknown-linux-gnu
export R5_TARGET=riscv64-unknown-linux-gnu

## RISC-V GNU Compiler Toolchain
https://github.com/riscv-collab/riscv-gnu-toolchain

### Build rv64gc toolchain with LLVM
./configure --prefix=$R5 --enable-llvm --enable-linux --with-arch=rv64gc --with-abi=lp64d
make -j$(nproc) all build-sim SIM=qemu

## Build Root
https://github.com/flutter/flutter/wiki/Setting-up-the-Engine-development-environment

## Update Flutter Engine
cd src/flutter
git pull upstream main

## Configure

cd src

### GN

./flutter/tools/gn --target-os linux --linux-cpu riscv64 --target-sysroot /opt/riscv/sysroot --target-toolchain /opt/riscv --target-triple riscv64-unknown-linux-gnu --no-prebuilt-dart-sdk --disable-desktop-embeddings --runtime-mode debug --embedder-for-target --unoptimized --no-lto --no-stripped --no-goma

ninja -C out/linux_debug_unopt_riscv64

## Dependencies
https://gitee.com/openeuler/bishengjdk-11/blob/risc-v/DEPENDENCY_BUILD.md

### pkg-config

./configure --prefix=/opt/riscv/sysroot/usr --with-internal-glib --disable-host-tool --docdir=/opt/riscv/sysroot/usr/share/doc/pkg-config-0.29.2 --host=$R5_HOST --target=$R5_TARGET

make && make install

### freetype2
./configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX --with-brotli=no --with-harfbuzz=no --with-bzip2=no PKG_CONFIG_PATH=$R5_PREFIX/lib/pkgconfig

make && make install

### zlib
PKG_CONFIG_PATH=$R5_PKG_CONFIG_PATH CHOST=riscv64 CC=riscv64-unknown-linux-gnu-gcc AR=riscv64-unknown-linux-gnu-ar RANLIB=riscv64-unknown-linux-gnu-ranlib ./configure  --prefix=$R5_PREFIX

make && make install

### libpng
export CC=riscv64-unknown-linux-gnu-gcc \
export CPPFLAGS="-I$R5_PREFIX/include" \
export LDFLAGS="-L$R5_PREFIX/lib"

./configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX

make && make install

### xcb-proto
./configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX

make && make install

### libXcb
./configure --host=$R5_HOST --target=$R5_TARGET --disable-malloc0returnsnull --prefix=$R5_PREFIX PKG_CONFIG_PATH=$R5_PREFIX/lib/pkgconfig

make && make install

### libgbd
meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX

ninja -C build/ install

### libexpat
git clone https://github.com/libexpat/libexpat
cd libexpat/expat
./buildconf.sh
PKG_CONFIG_PATH=$R5_PKG_CONFIG_PATH ./configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX

make && make install

### libffi
git clone https://github.com/libffi/libffi
cd libffi
./autogen.sh
mkdir target
cd target
PKG_CONFIG_PATH=$R5_PKG_CONFIG_PATH ../configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX

make && make install

### libxml2
git clone https://github.com/GNOME/libxml2
cd libxml2
./autogen.sh
PKG_CONFIG_PATH=$R5_PKG_CONFIG_PATH ./configure --host=$R5_HOST --target=$R5_TARGET --prefix=$R5_PREFIX --without-python

make && make install

### wayland
meson setup build/ --prefix=$R5_PREFIX -Ddocumentation=false
ninja -C build/ install

export PKG_CONFIG_LIBDIR=$R5_PKG_CONFIG_PATH
meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX -Ddocumentation=false
ninja -C build/ install

### wayland-protocols
export PATH=$R5/bin:$PATH
PKG_CONFIG_PATH=$R5_PKG_CONFIG_PATH meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX

ninja -C build/ install

### mesa
#### vulkan
meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX -Dgallium-drivers=swrast -Dvulkan-drivers="" -Dglx=disabled -Dplatforms=wayland -Dgbm=enabled -Degl=enabled
#### opengl
meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX -Dgallium-drivers=swrast -Dvulkan-drivers="" -Dplatforms=wayland -Dgbm=enabled -Degl=enabled -Degl-native-platform=drm

meson setup build/ --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX -Dgallium-drivers=swrast -Dvulkan-drivers="" -Dglx=disabled -Dplatforms=wayland -Dgbm=enabled -Degl=enabled -Degl-native-platform=drm

ninja -C build/ install

### libxkbcommon
git clone https://github.com/xkbcommon/libxkbcommon

meson setup build \
    -Denable-x11=false \
    -Dxkb-config-root=/usr/share/X11/xkb \
    -Dx-locale-root=/usr/share/X11/locale \
    -Denable-docs=false \
    --cross-file=riscv64-linux-gnu.ini --prefix=$R5_PREFIX

meson compile -C build
meson install -C build

### LLVM
https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm
https://llvm.org/docs/HowToCrossCompileLLVM.html

cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$R5_CMAKE_TOOLCHAIN_FILE \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_INSTALL_PREFIX=$R5_PREFIX \
    -DLLVM_HOST_TRIPLE=riscv64-unknown-linux-gnu \
    -DLLVM_TARGETS_TO_BUILD=RISCV \
    -DCMAKE_CXX_FLAGS='--sysroot=${R5_SYSROOT}'

ninja -C build
ninja -C build install

### DBUS
https://gitlab.freedesktop.org/dbus/dbus