#!/bin/sh

SECTION=""

die() {
    printf "\033[31mFailed on $SECTION step.\n\033[0m"
    exit 1
}

SECTION="CMake configuration"
cmake . || die

SECTION="C++ build"
cmake --build . || die

SECTION="Cython build"
sh cython_build.sh || die

printf "\033[32mBuilt successfully.\n\033[0m"
exit 0