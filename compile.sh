#!/bin/bash
set -eux
g++ $1 -std=c++17 -g -O2 -I${HOME}/kyopuro/lib
