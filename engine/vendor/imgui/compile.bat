@echo off

set sourceFiles=*.cpp
set compilerFlags=-fuse-ld=llvm-lib -std=c++17 -o lib/imgui.lib

clang++ %sourceFiles% %compilerFlags%