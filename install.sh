#!/usr/bin/env bash

if [ ! -d build ]; then
  echo "Project has not been built yet."
  ./build.sh;
fi
cd build
echo "Installing.."
make install > /dev/null 2>&1
echo "Done."
