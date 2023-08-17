set(CMAKE_SYSTEM_PROCESSOR riscv)

set(R5_HOST_TAG linux)

set(R5_ROOT "$ENV{R5}" CACHE PATH "RISC-V tool path")

set(R5_TOOLCHAIN_ROOT "${R5_ROOT}" CACHE PATH "RISC-V compiler path")
set(R5_TOOLCHAIN_PREFIX "riscv64-unknown-linux-gnu-" CACHE STRING "RISC-V toolchain prefix")
set(CMAKE_FIND_ROOT_PATH ${R5_TOOLCHAIN_ROOT})
list(APPEND CMAKE_PREFIX_PATH "${R5_TOOLCHAIN_ROOT}")

set(CMAKE_C_COMPILER "${R5_TOOLCHAIN_ROOT}/bin/clang")
set(CMAKE_CXX_COMPILER "${R5_TOOLCHAIN_ROOT}/bin/clang++")
set(CMAKE_AR "${R5_TOOLCHAIN_ROOT}/bin/llvm-ar")
set(CMAKE_RANLIB "${R5_TOOLCHAIN_ROOT}/bin/llvm-ranlib")
set(CMAKE_STRIP "${R5_TOOLCHAIN_ROOT}/bin/llvm-strip")

set(R5_COMPILER_FLAGS)
set(R5_COMPILER_FLAGS_CXX)
set(R5_COMPILER_FLAGS_DEBUG)
set(R5_COMPILER_FLAGS_RELEASE)
set(R5_LINKER_FLAGS)
set(R5_LINKER_FLAGS_EXE)