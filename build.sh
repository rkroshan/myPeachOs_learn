#!/bin/bash

#preparation exports
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

#Run our Make commands
if [ -z $1 ]; then
    make all
else
    export MAKE_TARGET_CMD=$1
    make all_r
fi
