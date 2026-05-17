#!/bin/bash

if [ ! -d build/ ]; then
    mkdir -p build;
fi

CompileFlags="
    -g \
    -O2 \
    -std=c11 \
    -fPIC \
    -ffreestanding \
    -fno-stack-protector \
    -Wall -Wextra -Wpedantic -Werror \
    -Wno-unused-variable \
    -Wno-unused-function \
    -Wno-unused-parameter \
    -nostdlib"

LinkFlags="
    -fuse-ld=lld \
    -Wl,-nostdlib \
    -Wl,-entry,LinuxEntry"

SourceFile="code/linux.c"
OutputFile="build/whatever"

clang $CompileFlags $LinkFlags -o $OutputFile $SourceFile
