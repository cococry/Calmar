@echo off

setlocal enabledelayedexpansion

pushd ..\src
set sourceFiles=
for /R %%f in (*.cpp) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

pushd ..\

set thirdpartyFiles=vendor/glad/src/glad.cpp vendor/stb_image/stb_image.cpp 
set includeDirs=-Isrc -Ivendor/glfw/include -Ivendor/glad/include -Ivendor/glm -Ivendor/stb_image -Ivendor/spdlog/include
set generalLinkingFlags=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lmsvcrtd.lib -llibcmt
set linkerFlags=%generalLinkingFlags% -lopengl32 -Lvendor/glfw/lib -lglfw3
set compilerFlags=-g -shared -Wvarargs -Wall -o ../bin/calmar_engine.dll
set preDefinedMacros=-D_DEBUG -DCALMAR_EXPORT -D_CRT_SECURE_NO_WARNINGS -DGLFW_INCLUDE_NONE

echo Starting Calmar Engine build process...

clang++ %sourceFiles% %thirdpartyFiles% %includeDirs% %linkerFlags% %preDefinedMacros% %compilerFlags%
popd