#!/bin/sh

cmake .
cmake --build .
sh cython_build.sh