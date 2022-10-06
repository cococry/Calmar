@echo off

setlocal enabledelayedexpansion

pushd ..\src
set sourceFiles=
for /R %%f in (*.cpp) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

pushd ..\

set thirdpartyFiles=vendor/glad/src/glad.cpp vendor/glad/src/glad_wgl.cpp vendor/stb_image/stb_image.cpp 
set includeDirs=-Isrc -Ivendor/glfw/include -Ivendor/glad/include -Ivendor/glm -Ivendor/stb_image -Ivendor/spdlog/include -Ivendor/gllib -I%VULKAN_SDK%/Include
set linkerFlags=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lopengl32 -Lvendor/glfw/lib -L%VULKAN_SDK%/Lib -lglfw3_mt -lvulkan-1 
set compilerFlags=-g -shared -Wvarargs -Wall -std=c++17 -o ../bin/CalmarEngine.dll
set preDefinedMacros=-DCALMAR_EXPORT -D_CRT_SECURE_NO_WARNINGS -DGLFW_INCLUDE_NONE -DDEBUG_BUILD

echo Starting Calmar Engine build process...

clang++ %sourceFiles% %thirdpartyFiles% %includeDirs% %linkerFlags% %preDefinedMacros% %compilerFlags%
popd
