#!/bin/sh

if [ ! -e Makefile ]; then
  make clean
fi
cmake -DGeant4_DIR=$G4LIB ../SpiRIT_G4simulator
make -j2 
