#!/bin/bash

#preparation exports
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

#Run our Make commands
make all