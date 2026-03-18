#!/bin/bash

BUILD_SHARED_LIBS="OFF"
FULLY_STATIC="ON"
MUSL_ARCH="x86_64"

export CC=`pwd`/"$MUSL_ARCH"-linux-musl-native/bin/"$MUSL_ARCH"-linux-musl-gcc
export CXX=`pwd`/"$MUSL_ARCH"-linux-musl-native/bin/"$MUSL_ARCH"-linux-musl-g++

cmake \
  -DCMAKE_BUILD_TYPE="Release" \
  -DBUILD_SHARED_LIBS="$BUILD_SHARED_LIBS" \
  -DFULLY_STATIC="$FULLY_STATIC" \
  -S . \
  -B build/
