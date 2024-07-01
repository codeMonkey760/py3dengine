#! /usr/bin/env bash

PIP_EXE='venv/bin/pip3'
MATH_DIR='py3dengine/math'

if [ ! -f $PIP_EXE ]; then
  echo 'Current working directory is wrong or virtual env is not set up';
  exit 1
fi

if ! cd $MATH_DIR/lib; then
  echo 'Could not cd into math library'
  exit 1
fi

mkdir cmake-build-debug
cmake -S . -B cmake-build-debug
if ! cd cmake-build-debug; then
  echo 'Could not cd into cmake folder for math library';
  exit 1
fi

make

cd ../../../..

$PIP_EXE install py3dengine/math/extension
$PIP_EXE install .