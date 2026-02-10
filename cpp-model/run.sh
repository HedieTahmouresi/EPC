#!/usr/bin/env bash
set -e

mkdir -p build
rm -rf build/*
cd build

cmake ..
make -j"$(nproc)"

./epc_run
