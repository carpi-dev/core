#!/usr/bin/env bash

if [ ! -d "build" ]; then mkdir build; else (cd build; make clean); fi
cd build
echo "Running CMake."
cmake .. > /dev/null 2>&1
echo "Running Make."
make -j ($nproc) > /dev/null 2>&1
echo "Built project."