@echo off

set sourceFiles=src/*.cpp src/contrib/*.cpp -Iinclude
set compilerFlags=-fuse-ld=llvm-lib -std=c++17 -o lib/yaml-cpp.lib -DYAML_CPP_STATIC_DEFINE 

clang++ %sourceFiles% %compilerFlags%