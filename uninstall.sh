#! /usr/bin/env bash

PIP_EXE='venv/bin/pip3'

if [ ! -f $PIP_EXE ]; then
  echo 'Current working directory is wrong or virtual env is not set up';
  exit 1
fi

$PIP_EXE uninstall -y py3dengine.math
$PIP_EXE uninstall -y py3dengine