@echo off

setlocal enabledelayedexpansion

pushd ..\src
set sourceFiles=
for /R %%f in (*.cpp) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

pushd ..\

set includeDirs=-Isrc -I../engine/src -I../engine/vendor/glfw/include -I../engine/vendor/glad/include -I../engine/vendor/glm -I../engine/vendor/stb_image -I../engine/vendor/spdlog/include
set generalLinkingFlags=-lglfw3_mt -lcalmar_engine  -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lopengl32
set linkerFlags=-L../bin -L../engine/vendor/glfw/lib  %generalLinkingFlags%
set compilerFlags=-Wvarargs -Wall -o ../bin/calmar_editor.exe
set preDefinedMacros=-DCALMAR_IMPORT -D_CRT_SECURE_NO_WARNINGS -DGLFW_INCLUDE_NONE

echo Starting Calmar Editor build process...

clang++ %sourceFiles% %thirdpartyFiles% %includeDirs% %linkerFlags% %preDefinedMacros% %compilerFlags%
popd