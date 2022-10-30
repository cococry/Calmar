@echo off

set sourceFiles=src/collision/*.cpp src/common/*.cpp src/dynamics/*.cpp src/rope/*.cpp
set includeDirs=-Iinclude
set compilerFlags=-fuse-ld=llvm-lib -std=c++17 -o lib/box2d.lib

clang++ %sourceFiles% %compilerFlags% %includeDirs%