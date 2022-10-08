@echo off

setlocal enabledelayedexpansion

pushd ..\src
set sourceFiles=
for /R %%f in (*.cpp) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

pushd ..\

set thirdpartyFiles=vendor/glad/src/glad.cpp vendor/glad/src/glad_wgl.cpp vendor/stb_image/stb_image.cpp vendor/imgui/*.cpp
set includeDirs=-Isrc -Ivendor/glfw/include -Ivendor/glad/include -Ivendor/glm -Ivendor/stb_image -Ivendor/spdlog/include -I%VULKAN_SDK%/Include -Ivendor/imgui
set linkerFlags=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lopengl32 -Lvendor/glfw/lib -Lvendor/imgui/lib -L%VULKAN_SDK%/Lib -lglfw3_mt -lvulkan-1  -lImGui
set compilerFlags=-g -shared -Wvarargs -Wall -std=c++17 -o ../bin/CalmarEngine.dll
set preDefinedMacros=-DCALMAR_EXPORT -D_CRT_SECURE_NO_WARNINGS -DGLFW_INCLUDE_NONE

echo Starting Calmar Engine build process...

clang++ %sourceFiles% %thirdpartyFiles% %includeDirs% %linkerFlags% %preDefinedMacros% %compilerFlags%
popd
