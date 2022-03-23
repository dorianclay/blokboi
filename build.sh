#!/bin/bash

SECTION=""

die() {
    printf "\033[31mFailed on $SECTION step.\n\033[0m"
    exit 1
}

if [[ $1 ]]; then
    if [ $1 = "clean" ]; then
        SECTION="Cython clean"
        python3 setup.py clean --all || die
        SECTION="make clean"
        make clean || die
        printf "\033[33mSuccessfully cleaned cython and c++ build files.\n\033[0m"
    else
        printf "Current options are:\n"
        printf "\tclean\tRemove all build files.\n"
    fi
else 
    SECTION="CMake configuration"
    cmake . || die

    SECTION="C++ build"
    cmake --build . || die

    SECTION="Cython build"
    sh cython_build.sh || die

    printf "\033[32mBuilt successfully.\n\033[0m"
fi

exit 0