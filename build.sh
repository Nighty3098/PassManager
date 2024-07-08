#!/bin/bash

rm -rf src/PassSafe/build
cd src/PassSafe
mkdir build
cmake .
make
