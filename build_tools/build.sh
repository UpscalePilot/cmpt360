#!/bin/bash
#quick build 
#

mkdir $1

mkdir $1/bin
mkdir $1/build
mkdir $1/doc
mkdir $1/src
mkdir $1/obj


cp ./build_tools/Templates/README.md $1
cp ./build_tools/Templates/main.c $1/src/
cp ./build_tools/Templates/Makefile $1

chmod -R 755 $1
chmod 644 $1/README.md
chmod 644 $1/Makefile
chmod -R 644 $1/src/*


tree -p $1
echo -e "\n\nscript complete\n\n\n"
